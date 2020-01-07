#include "b_tree.h"
#include "b_tree_utils.h"

static bt_node *init_new_node(int val)
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

static void __insert_to_tree(int val, bt_node **node);

static void insert_to_child(int val, bt_node *node)
{
	int i;
	for (i = 0; i < node->cnt; i++) {
		if (val < node->key[i]) {
			break;
		}
	}
	DBG_PRINT("Inserting in child [%d]\n", i);
	__insert_to_tree(val, &node->child[i]);
	if (node->child[i])
		node->child[i]->pptr = node;
	return;
}

/* 
 * This function is to be called only when 
 * there is space in the node to add a new key
 * Undefined behavior if NULL or full
 */
static void insert_to_node_sorted(int val, bt_node *node)
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

static void split_and_insert(int val, bt_node *node)
{
	int i, mid, new_cnt;
	bt_node *sibling_node, *parent;
	mid = node->key[deg-1];
	DBG_PRINT("Splitting with mid = %d\n", mid);
	
	if (node->pptr == NULL) {
		__insert_to_tree(mid, &node->pptr);
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

static void __insert_to_tree(int val, bt_node **node)
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

void insert_to_tree(int val, bt_node **node)
{
	__insert_to_tree(val, node);

	while((*node)->pptr) { // Go to root
		(*node) = (*node)->pptr;
	}
	return;
}
