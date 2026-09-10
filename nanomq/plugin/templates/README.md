# Stream Plugin 模板目录

这个目录只保留一个目的：提供最小可编译模板，帮助你快速产出 `my_stream_plugin.so`。

## 保留文件

- `skeleton.c`：最小插件骨架（`on_msg` / `on_start` / `on_stop` + `nano_plugin_init`）
- `Makefile`：把 `skeleton.c` 编译为 `my_stream_plugin.so`，并自动链接 `nano_skill` 相关源码
- `README.md`：本说明

> 需要完整示例时，请直接看 `../can_pipeline_sample.c`。

## 快速使用

1. 在仓库根目录编译 NanoMQ（首次）：

```bash
cd build
cmake .. -DENABLE_PLUGIN=ON
make -j"$(nproc)" nanomq nanomq_cli
```

1. 在本目录编译模板插件：

```bash
make NMQ_INCLUDE=../../include
```

产物：`./my_stream_plugin.so`

2.5) 一键自验证（推荐）：

```bash
make verify NMQ_INCLUDE=../../include
```

说明：

- `verify` 会调用 `./self_verify.sh`
- 自动流程：启动临时 NanoMQ -> 订阅输出 topic -> 发布输入消息 -> 校验输出与落盘 -> 返回退出码
- 默认验证 `metrics/example`，脚本会按 5 秒窗口节奏发布样例消息；如需验证逐条回发，可执行 `OUT_TOPIC=stream_forwarded ./self_verify.sh`
- 成功返回 `0`，失败返回非 `0`，适合 CI/脚本集成

模板插件当前包含两条输出链路：

- `stream_forwarded`：每收到一条 `canudp` 输入就立即回灌到 Broker。
- `metrics/example`：由插件内部定时线程每 5000ms 主动 flush 当前窗口；窗口无数据时也发布 `count=0`。

1. 写最小配置并加载插件：

```bash
cat > /tmp/nmq-sp.conf <<'EOF'
listeners.tcp { bind = "0.0.0.0:11884" }
log { to = [console] level = info }

stream_plugin {
  sp0 {
    path  = "/ABS/PATH/TO/my_stream_plugin.so"
    topic = "canudp"
    mode  = "async"
    queue_cap = 4096
    full_op   = "drop"
  }
}

stream_inject {
  enable = true
  queue_cap = 4096
  worker_num = 1
  full_op = "drop"
}
EOF
```

把路径替换为真实 `.so` 绝对路径后执行：

```bash
./../../build/nanomq/nanomq start --conf /tmp/nmq-sp.conf
```

手工验证时，订阅 `metrics/example` 可观察 5 秒定时窗口输出；订阅 `stream_forwarded` 可观察逐条回发。

更完整的端到端演示可参考 `docs/zh_CN/stream-plugin/quick-demo.md`。