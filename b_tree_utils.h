#ifndef __B_TREE_UTILS_H__
#define __B_TREE_UTILS_H__

#include "b_tree.h"

bool is_node_half(bt_node *node);
bool is_node_root(bt_node *node);
bool is_node_full(bt_node *node);
bool is_node_leaf(bt_node *node);

int height(bt_node *node);
int max_of_tree(bt_node *node);
int min_of_tree(bt_node *node);
#endif // __B_TREE_UTILS_H__