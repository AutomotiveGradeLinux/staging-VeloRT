//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_LIST_H
#define CORE_LIST_H

#include "core/defs.h"

// In order to make life easy, we just define the list structures
// directly, and let consumers directly inline structures.
typedef struct nni_list_node {
	struct nni_list_node *ln_next;
	struct nni_list_node *ln_prev;
} nni_list_node;

typedef struct nni_list {
	struct nni_list_node ll_head;
	size_t               ll_offset;
} nni_list;

extern void nni_list_init_offset(nni_list *list, size_t offset);

#define NNI_LIST_INIT(list, type, field) \
	nni_list_init_offset(list, offsetof(type, field))

// NNI_LIST_INITIALIZER is used to initialize structures at declaration time.
// The list argument is the structure being initialized and the type and
// offset determine where the node lives within each item.
#define NNI_LIST_INITIALIZER(list, type, field)          \
	{                                                \
		.ll_head.ln_next = &(list).ll_head,      \
		.ll_head.ln_prev = &(list).ll_head,      \
		.ll_offset       = offsetof(type, field) \
	}

#define NNI_LIST_NODE_INIT(node)                       \
	do {                                           \
		(node)->ln_prev = (node)->ln_next = 0; \
	} while (0)

extern void *nni_list_first(const nni_list *);
extern void *nni_list_last(const nni_list *);
extern void  nni_list_append(nni_list *, void *);
extern void  nni_list_prepend(nni_list *, void *);
extern void  nni_list_insert_before(nni_list *, void *, void *);
extern void  nni_list_insert_after(nni_list *, void *, void *);
extern void *nni_list_next(const nni_list *, void *);
extern void *nni_list_prev(const nni_list *, void *);
extern void  nni_list_remove(nni_list *, void *);
extern int   nni_list_active(nni_list *, void *);
extern int   nni_list_empty(nni_list *);
extern int   nni_list_node_active(nni_list_node *);
extern void  nni_list_node_remove(nni_list_node *);

#define NNI_LIST_FOREACH(l, it) \
	for (it = nni_list_first(l); it != NULL; it = nni_list_next(l, it))

#endif // CORE_LIST_H
