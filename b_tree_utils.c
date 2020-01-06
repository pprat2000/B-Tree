#include "b_tree.h"

extern inline bool is_node_half(bt_node *node) {
	if (node == NULL)
		return true;
	return (node->cnt <= deg - 1) ? true : false;
}

extern inline bool is_node_root(bt_node *node) {
	return (node->pptr) ? false : true;
}

extern inline bool is_node_full(bt_node *node) {
	return (node->cnt == 2*deg - 1) ? true : false; 
}

extern inline bool is_node_leaf(bt_node *node) {
	return node->child[0] ? false : true;
}

int max_of_tree(bt_node *node)
{
	if (node == NULL) {
		DBG_PRINT("Node is NULL. Exiting!\n");
		exit(1);
	}

	if (is_node_leaf(node))
		return node->key[node->cnt-1];
	else
		return max_of_tree(node->child[node->cnt]);

}

int min_of_tree(bt_node *node)
{
	if (node == NULL) {
		DBG_PRINT("Node is NULL. Exiting!\n");
		exit(1);
	}

	if (is_node_leaf(node))
		return node->key[0];
	else
		return min_of_tree(node->child[0]);

}

int height(bt_node *node)
{
	if (node == NULL)
		return 0;
	else
		return height(node->child[0]) + 1;
}
