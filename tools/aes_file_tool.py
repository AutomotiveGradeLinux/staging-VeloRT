#!/usr/bin/env python3
import argparse
import base64
import sys
from pathlib import Path

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

AES_GCM_AAD = bytes([
    0x4D, 0x23, 0xC3, 0xCE, 0xC3, 0x34, 0xB4, 0x9B,
    0xDB, 0x37, 0x0C, 0x43, 0x7F, 0xEC, 0x78, 0xDE,
])
AES_GCM_IV = bytes([
    0x99, 0xAA, 0x3E, 0x68, 0xED, 0x81, 0x73, 0xA0,
    0xEE, 0xD0, 0x66, 0x84,
])


def aes_gcm_expected_key_len(encrypt_method: str) -> int:
    if not encrypt_method:
        raise ValueError("encrypt_method is empty")

    method = encrypt_method.upper()
    if method == "AES128-GCM":
        return 16
    if method == "AES192-GCM":
        return 24
    if method == "AES256-GCM":
        return 32
    raise ValueError(f"Unsupported encrypt method: {encrypt_method}")


def aes_gcm_get_method_by_key_len(key_len: int) -> str:
    if key_len == 16:
        return "AES128-GCM"
    if key_len == 24:
        return "AES192-GCM"
    if key_len == 32:
        return "AES256-GCM"
    raise ValueError(f"Invalid key length: {key_len}")


def aes_gcm_validate_key(key: bytes, encrypt_method: str) -> None:
    expected = aes_gcm_expected_key_len(encrypt_method)
    if key is None:
        raise ValueError("AES key is NULL")
    if len(key) != expected:
        raise ValueError(
            f"AES key length mismatch for {encrypt_method}: expected {expected}, got {len(key)}"
        )


def nni_aes_gcm_encrypt(plain: bytes, key: bytes, encrypt_method: str) -> bytes:
    aes_gcm_validate_key(key, encrypt_method)

    encryptor = Cipher(
        algorithms.AES(key),
        modes.GCM(AES_GCM_IV),
        backend=default_backend(),
    ).encryptor()

    encryptor.authenticate_additional_data(AES_GCM_AAD)
    ciphertext = encryptor.update(plain) + encryptor.finalize()
    tag16 = encryptor.tag

    # Keep the exact C format: 32-byte header where first 16 bytes are the GCM tag.
    tag32 = tag16 + b"\x00" * 16
    return tag32 + ciphertext


def nni_aes_gcm_decrypt(cipher_blob: bytes, key: bytes, encrypt_method: str) -> bytes:
    aes_gcm_validate_key(key, encrypt_method)

    if len(cipher_blob) < 32:
        raise ValueError(f"cipher text size {len(cipher_blob)} is invalid (too short)")

    tag32 = cipher_blob[:32]
    tag16 = tag32[:16]
    ciphertext = cipher_blob[32:]

    decryptor = Cipher(
        algorithms.AES(key),
        modes.GCM(AES_GCM_IV, tag16),
        backend=default_backend(),
    ).decryptor()

    decryptor.authenticate_additional_data(AES_GCM_AAD)
    return decryptor.update(ciphertext) + decryptor.finalize()


def load_key_from_args(args: argparse.Namespace) -> bytes:
    provided = [
        args.key_text is not None,
        args.key_hex is not None,
        args.key_b64 is not None,
    ]
    if sum(provided) != 1:
        raise ValueError("Exactly one of --key-text, --key-hex, --key-b64 must be provided")

    if args.key_text is not None:
        return args.key_text.encode("utf-8")
    if args.key_hex is not None:
        return bytes.fromhex(args.key_hex)
    return base64.b64decode(args.key_b64, validate=True)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="AES-GCM file encrypt/decrypt compatible with NanoMQ C format"
    )
    parser.add_argument("--mode", choices=["enc", "dec"], required=True, help="enc or dec")
    parser.add_argument("--in", dest="infile", required=True, help="input file path")
    parser.add_argument("--out", dest="outfile", required=True, help="output file path")

    parser.add_argument(
        "--method",
        dest="encrypt_method",
        default=None,
        help="AES128-GCM / AES192-GCM / AES256-GCM. If omitted, inferred from key length.",
    )

    parser.add_argument("--key-text", dest="key_text", default=None, help="key as UTF-8 text")
    parser.add_argument("--key-hex", dest="key_hex", default=None, help="key as hex string")
    parser.add_argument("--key-b64", dest="key_b64", default=None, help="key as base64 string")
    return parser.parse_args()


def main() -> int:
    args = parse_args()

    try:
        key = load_key_from_args(args)
        method = args.encrypt_method if args.encrypt_method else aes_gcm_get_method_by_key_len(len(key))

        in_path = Path(args.infile)
        out_path = Path(args.outfile)
        data = in_path.read_bytes()

        if args.mode == "enc":
            result = nni_aes_gcm_encrypt(data, key, method)
        else:
            result = nni_aes_gcm_decrypt(data, key, method)

        out_path.write_bytes(result)
        print(f"OK: {args.mode} -> {out_path} ({len(result)} bytes), method={method}")
        return 0
    except Exception as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
