#include "b_tree.h"

int main()
{
	int i;
	bt_node *b_tree = NULL;

	for(i = 10; i <= 500; i += 10) {
		insert_to_tree(i, &b_tree);
		while (b_tree->pptr) { // Go to root
			b_tree = b_tree->pptr;
		}
	}
	display_tree(b_tree);

	for (i = 20; i < 400; i+=10) {
		delete_from_tree(i, &b_tree);
	}

	display_tree(b_tree);

	printf("Enter a Value to Search: ");
	scanf("%d", &i);

	if (search_in_tree(i, b_tree))
		printf("Found value %d in tree!\n", i);
	else
		printf("Value %d not found!\n", i);

	return 0;
}