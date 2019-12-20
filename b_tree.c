#include "b_tree.h"

static inline bool is_node_full(bt_node *node) {
	return (node->cnt == 2*deg - 1) ? true : false; 
}

static inline bool is_node_leaf(bt_node *node) {
	return node->child[0] ? false : true;
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
 */
void insert_to_node_sorted(int val, bt_node *node)
{
	int i, j;

	if (is_node_full(node)) {
		DBG_PRINT("Trying to insert in a full node!\n");
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
	int i, mid;
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
	sibling_node = init_new_node(val);
	for (i = deg; i < node->cnt; i++) {
		insert_to_node_sorted(node->key[i], sibling_node);
	}
	if (!is_node_leaf(node)) { // Take care of children of node
		for (i = deg; i <= node->cnt; i++) { // <= because 1 more child than key
			sibling_node->child[i-deg] = node->child[i];
			node->child[i] = NULL;
			sibling_node->child[i-deg]->pptr = sibling_node;
		}
	}
	node->cnt = deg-1;
	parent = node->pptr;
	sibling_node->pptr = parent;
	for (i = 0; i <= parent->cnt; i++) {
		if (parent->child[i] == NULL)
			break;
	}
	parent->child[i] = sibling_node;
	return;
}

void insert_to_tree(int val, bt_node **node)
{
	int i;
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

int main()
{
	int i;
	bt_node *b_tree = NULL;

	for(i = 10; i <= 200; i += 10) {
		insert_to_tree(i, &b_tree);
		while (b_tree->pptr) { // Go to root
			b_tree = b_tree->pptr;
		}
	}
	return 0;
}