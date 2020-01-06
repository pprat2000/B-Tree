#include "b_tree.h"
#include "b_tree_utils.h"

/*
 * This function searches for the tree
 * for a value and returns true/false
 * depending on the result.
 */
bool search_in_tree(int val, bt_node *node)
{
	int i;
	bool found;

	if (node == NULL) {
		DBG_PRINT("Trying to search in an empty node! ERROR!\n");
		exit(1);
	}

	for (i = 0; i < node->cnt; i++) { // Finding the idx
		if (val <= node->key[i])
			break;
	}

	if (i < node->cnt && node->key[i] == val) { // If val in this node
		DBG_PRINT("Value Found in Node with 0th idx value %d\n", node->key[0]);
		found = true;
	} else { // If val not in this node
		if (is_node_leaf(node)) {
			DBG_PRINT("%d not in the Tree!\n", val);
			found = false;
		} else {
			DBG_PRINT("Value not found in node! Traversing child [%d]!\n", i);
			found = search_in_tree(val, node->child[i]);
		}
	}

	return found;
}
