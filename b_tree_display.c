#include "b_tree.h"
#include "b_tree_utils.h"

static void __display_tree(bt_node *node, int h)
{
	int i;

	if (node == NULL)
		return;
	if (h == 1) {
		for (i = 0; i < node->cnt; i++)
			printf("%d  ", node->key[i]);
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
		printf("\n*******************************\n");
	}
	return;
}

