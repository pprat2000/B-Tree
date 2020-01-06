#ifndef __B_TREE_UTILS_H__
#define __B_TREE_UTILS_H__

#include "b_tree.h"

static inline bool is_node_half(bt_node *node) {
	if (node == NULL)
		return true;
	return (node->cnt <= deg - 1) ? true : false;
}

static inline bool is_node_root(bt_node *node) {
	return (node->pptr) ? false : true;
}

static inline bool is_node_full(bt_node *node) {
	return (node->cnt == 2*deg - 1) ? true : false; 
}

static inline bool is_node_leaf(bt_node *node) {
	return node->child[0] ? false : true;
}

#endif // __B_TREE_UTILS_H__