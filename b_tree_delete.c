#include "b_tree.h"
#include "b_tree_utils.h"

/* 
 * This function is to be called only
 * when it is a leaf node and there node
 * is not underflowing. Undefined behavior
 * otherwise!
 */
static bool delete_from_leaf(int val, bt_node *node, int idx)
{
	int i;

	if (is_node_half(node) && !is_node_root(node)) {
		DBG_PRINT("Trying to delete from half full node! ERROR!\n");
		exit(1);
	}

	// val in idx. Moving everything else
	for (i = idx; i < node->cnt-1; i++) {
		node->key[i] = node->key[i+1];
	}
	node->cnt--;
	DBG_PRINT("Deleted from index [%d]\n", idx);
	return true;
}

/*
 * This function merges left and right
 * child of the node if both are underflowing
 */
static void merge_siblings(bt_node *node, bt_node *left, bt_node *right, int idx)
{
	int i;

	DBG_PRINT("Merging child with sibling\n");

	left->key[left->cnt] = node->key[idx];
	left->cnt++; // Makes the below loop easier

	for ( i = 0; i <= right->cnt; i++) {
		if (i < right->cnt)
			left->key[left->cnt+i] = right->key[i];
		left->child[left->cnt+i] = right->child[i];
	}
	left->cnt += right->cnt;
	free(right);

	for (i = idx; i < node->cnt-1 ; i++) { // Move everything to the left in node
		node->key[i] = node->key[i+1];
		node->child[i+1] = node->child[i+2];
	}

	node->child[node->cnt] = NULL;
	node->cnt--;
	return;
}

static void balance_with_left(bt_node *node, bt_node *child, bt_node *sibling, int idx)
{
	int i, key;
	bt_node *sibling_child;
	
	DBG_PRINT("Balancing with left!\n");

	// Move key from left to node
	key = node->key[idx-1];
	node->key[idx-1] = sibling->key[sibling->cnt-1];
	sibling_child = sibling->child[sibling->cnt];

	for (i = child->cnt+1; i > 0; i--) { // Move everything to the right in child
		if (i-2 >= 0) // For keys
			child->key[i-1] = child->key[i-2];
		child->child[i] = child->child[i-1];
	}
	
	sibling->child[sibling->cnt] = NULL;
	sibling->cnt--;
	
	child->cnt++;
	child->key[0] = key;
	child->child[0] = sibling_child;
	return;
}

static void balance_with_right(bt_node *node, bt_node *child, bt_node *sibling, int idx)
{
	int i, key;
	bt_node *sibling_child;
	
	DBG_PRINT("Balancing with right!\n");

	// Move key from left to node
	key = node->key[idx];
	node->key[idx] = sibling->key[0];
	sibling_child = sibling->child[0];

	for (i = 0; i < sibling->cnt; i++) { // Move everything to the left in sibling
		if (i < sibling->cnt-1) // For keys
			sibling->key[i] = sibling->key[i+1];
		sibling->child[i] = sibling->child[i+1];
	}
	
	sibling->child[sibling->cnt] = NULL;
	sibling->cnt--;
	
	child->cnt++;
	child->key[child->cnt-1] = key;
	child->child[child->cnt] = sibling_child;
	return;
}

static bool delete_from_internal_node(int val, bt_node *node, int idx)
{
	bool ret_val;
	bt_node *left_child, *right_child;
	
	left_child = node->child[idx];
	right_child = node->child[idx+1];

	if (!is_node_half(left_child)) {
		node->key[idx] = max_of_tree(left_child);
		DBG_PRINT("Deleted from index [%d]. Replaced with %d\n", idx, node->key[idx]);
		ret_val = delete_from_tree(node->key[idx], &left_child);
	} else if (!is_node_half(right_child)) {
		node->key[idx] = min_of_tree(right_child);
		DBG_PRINT("Deleted from index [%d]. Replaced with %d\n", idx, node->key[idx]);
		ret_val = delete_from_tree(node->key[idx], &right_child);
	} else { // We need to merge
		merge_siblings(node, left_child, right_child, idx);
		DBG_PRINT("Calling delete on the merged child\n");
		ret_val = delete_from_tree(val, &left_child);
	}

	return ret_val;
}

/* 
 * This function is to be called with idx of the child ptr
 * array of the node param where the value may reside
 */
static bool delete_from_child(int val, bt_node *node, int idx)
{
	bool ret_val;
	bt_node *child, *sibling_node_left, *sibling_node_right;

	child = node->child[idx];


	if (is_node_half(child)) { // Make it not half full

		DBG_PRINT("%d Child has only t-1 keys.\n", idx);
		
		// Special provision for first and last child
		sibling_node_left = (idx == 0) ? NULL : node->child[idx-1];
		sibling_node_right = (idx == node->cnt) ? NULL : node->child[idx+1];
		
		if (!is_node_half(sibling_node_left) || !is_node_half(sibling_node_right)) {
			// Take preference to left, then right
			if (!is_node_half(sibling_node_left))
				balance_with_left(node, child, sibling_node_left, idx);
			else
				balance_with_right(node, child, sibling_node_right, idx);
		} else {
			if (sibling_node_left) {
				// Freeing the right node hence switch the child pointer
				merge_siblings(node, sibling_node_left, child, idx-1);
				child = sibling_node_left;
			} else
				merge_siblings(node, child, sibling_node_right, idx);
		}
	}

	// Child node is not underflow anymore
	ret_val = delete_from_tree(val, &child);
	return ret_val;
}

bool delete_from_tree(int val, bt_node **node)
{
	int i;
	bool ret_val;
	bt_node *curr = *node;
	DBG_PRINT("Deleting Value: %d\n", val);

	if (curr == NULL) {
		DBG_PRINT("Node is NULL. Exiting!\n");
		exit(1);
	}

	for (i = 0; i < curr->cnt; i++) { // Finding the idx
		if (val <= curr->key[i])
			break;
	}

	if (i < curr->cnt && curr->key[i] == val) { // If val in this node
		if (is_node_leaf(curr)) {
			DBG_PRINT("Value in leaf!\n");
			ret_val = delete_from_leaf(val, curr, i);
		} else {
			DBG_PRINT("Value in internal node!\n");
			ret_val = delete_from_internal_node(val, curr, i);
		}
	} else { // If val not in this node
		if (is_node_leaf(curr)) {
			DBG_PRINT("%d not in the Tree! Ignoring delete command!\n", val);
			ret_val = false;
		} else {
			DBG_PRINT("Value not in curr node! Traversing child [%d]!\n", i);
			ret_val = delete_from_child(val, curr, i);
		}
	}

	// TODO - Hack as of now. More elegant solution
	while (curr && curr->cnt == 0) {
		*node = curr->child[0];
		if (*node)
			(*node)->pptr = NULL;
		free(curr);
		curr = *node;
	}
	return ret_val;
}
