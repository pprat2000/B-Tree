#include "b_tree.h"
#include "b_tree_utils.h"

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

static void __display_tree(bt_node *node, int h)
{
	int i;

	if (node == NULL)
		return;
	if (h == 1) {
		for (i = 0; i < node->cnt; i++)
			DBG_PRINT("%d  ", node->key[i]);
	} else {
		for (i = 0; i < node->cnt + 1; i++)
			__display_tree(node->child[i],h-1);
	}
	return;
}

void display_tree(bt_node *node)
{
	int i, h;
	h = height(node);
	for (i = 0; i < h; i++) {
		__display_tree(node, i+1);
		DBG_PRINT("\n*******************************\n");
	}
	return;
}

bt_node *init_new_node(int val)
{
	bt_node *node = NULL;
	node = malloc(sizeof(bt_node));
	if (node) {
		memset(node, 0, sizeof(bt_node));
		node->cnt = 1;
		node->key[0] = val;
	}
	return node;
}

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

/* 
 * This function is to be called only
 * when it is a leaf node and there node
 * is not underflowing. Undefined behavior
 * otherwise!
 */
void delete_from_leaf(int val, bt_node *node, int idx)
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
	return;
}

/*
 * This function merges left and right
 * child of the node if both are underflowing
 */
void merge_siblings(bt_node *node, bt_node *left, bt_node *right, int idx)
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

void balance_with_left(bt_node *node, bt_node *child, bt_node *sibling, int idx)
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

void balance_with_right(bt_node *node, bt_node *child, bt_node *sibling, int idx)
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

void delete_from_internal_node(int val, bt_node *node, int idx)
{
	bt_node *left_child, *right_child;
	
	left_child = node->child[idx];
	right_child = node->child[idx+1];

	if (!is_node_half(left_child)) {
		node->key[idx] = max_of_tree(left_child);
		DBG_PRINT("Deleted from index [%d]. Replaced with %d\n", idx, node->key[idx]);
		delete_from_tree(node->key[idx], &left_child);
	} else if (!is_node_half(right_child)) {
		node->key[idx] = min_of_tree(right_child);
		DBG_PRINT("Deleted from index [%d]. Replaced with %d\n", idx, node->key[idx]);
		delete_from_tree(node->key[idx], &right_child);
	} else { // We need to merge
		merge_siblings(node, left_child, right_child, idx);
		DBG_PRINT("Calling delete on the merged child\n");
		delete_from_tree(val, &left_child);
	}

	return;
}

/* 
 * This function is to be called with idx of the child ptr
 * array of the node param where the value may reside
 */
void delete_from_child(int val, bt_node *node, int idx)
{
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
	delete_from_tree(val, &child);
	return;
}

void delete_from_tree(int val, bt_node **node)
{
	int i;
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
			delete_from_leaf(val, curr, i);
		} else {
			DBG_PRINT("Value in internal node!\n");
			delete_from_internal_node(val, curr, i);
		}
	} else { // If val not in this node
		if (is_node_leaf(curr)) {
			DBG_PRINT("%d not in the Tree! Ignoring delete command!\n", val);
		} else {
			DBG_PRINT("Value not in curr node! Traversing child [%d]!\n", i);
			delete_from_child(val, curr, i);
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
	return;
}

void insert_to_child(int val, bt_node *node)
{
	int i;
	for (i = 0; i < node->cnt; i++) {
		if (val < node->key[i]) {
			break;
		}
	}
	DBG_PRINT("Inserting in child [%d]\n", i);
	insert_to_tree(val, &node->child[i]);
	if (node->child[i])
		node->child[i]->pptr = node;
	return;
}

/* 
 * This function is to be called only when 
 * there is space in the node to add a new key
 * Undefined behavior if NULL or full
 */
void insert_to_node_sorted(int val, bt_node *node)
{
	int i, j;

	if (is_node_full(node)) {
		DBG_PRINT("Trying to insert in a full node! ERROR!\n");
		exit(1);
	}
	for (i = 0; i < node->cnt; i++) { // Finding the idx
		if (val < node->key[i])
			break;
	}
	// val belongs to i index. Moving everything else
	for (j = node->cnt; j > i; j--) {
		node->key[j] = node->key[j-1];
	}
	DBG_PRINT("Inserting in index [%d]\n", i);
	node->key[i] = val;
	node->cnt++;
	return;
}

void split_and_insert(int val, bt_node *node)
{
	int i, mid, new_cnt;
	bt_node *sibling_node, *parent;
	mid = node->key[deg-1];
	DBG_PRINT("Splitting with mid = %d\n", mid);
	
	if (node->pptr == NULL) {
		insert_to_tree(mid, &node->pptr);
		node->pptr->child[0] = node;
	}
	else if (is_node_full(node->pptr)) {
		split_and_insert(mid, node->pptr);
	} else {
		insert_to_node_sorted(mid, node->pptr);
	}

	if (val <= mid) { // Put val in old node, cnt becomes deg and init sibling
		i = deg + 1;
		new_cnt = deg;
		node->key[deg-1] = val;
		sibling_node = init_new_node(node->key[deg]);
	} else { // Put val in sibling, cnt becomes deg - 1
		i = deg;
		new_cnt = deg-1;
		sibling_node = init_new_node(val);
	}
	for (; i < node->cnt; i++) {
		insert_to_node_sorted(node->key[i], sibling_node);
	}
	if (!is_node_leaf(node)) { // Take care of children of node
		for (i = deg; i <= node->cnt; i++) { // <= because 1 more child than key
			sibling_node->child[i-deg] = node->child[i];
			node->child[i] = NULL;
			sibling_node->child[i-deg]->pptr = sibling_node;
		}
	}
	node->cnt = new_cnt;
	parent = node->pptr;
	sibling_node->pptr = parent;
	for (i = parent->cnt-1; i >= 0; i--) { // Shift child ptr to the right & insert sib next to node
		if (parent->child[i] == node)
			break;
		parent->child[i+1] = parent->child[i];
	}
	parent->child[i+1] = sibling_node;
	return;
}

void insert_to_tree(int val, bt_node **node)
{
	bt_node *curr = NULL;
	DBG_PRINT("Inserting Value: %d\n", val);

	if (*node == NULL) { // Empty Tree
		*node = init_new_node(val);
	} else { // Non Empty Tree
		curr = *node;
		if (is_node_leaf(curr)) {
			if (is_node_full(curr)) {
				DBG_PRINT("Node is Full.\n");
				split_and_insert(val, curr);
			} else {
				DBG_PRINT("Node has space.\n");
				insert_to_node_sorted(val, curr);
			}
		} else { // If non-leaf, traverse and add
			insert_to_child(val, curr);
		}
	}
	return;
}
