#include "b_tree.h"

void print_menu()
{
	printf("1. Insert to Tree\n");
	printf("2. Search in Tree\n");
	printf("3. Delete from Tree\n");
	printf("4. Display the Tree\n");
	printf("-1 to Exit\n");
	printf("Enter your choice: ");
}

int main()
{
	int i, val;
	bt_node *b_tree = NULL;

	while (i != -1) {

		print_menu();
		scanf("%d", &i);

		switch (i)
		{
			case 1:
				printf("Enter the value : ");
				scanf("%d", &val);
				insert_to_tree(val, &b_tree);
				break;
			case 2:
				printf("Enter the value : ");
				scanf("%d", &val);
				if (search_in_tree(val, b_tree))
					printf("Found value %d in tree!\n", val);
				else
					printf("Value %d not found!\n", val);
				break;
			case 3:
				printf("Enter the value : ");
				scanf("%d", &val);
				delete_from_tree(val, &b_tree);
				break;
			case 4:
				display_tree(b_tree);
				break;
			case -1:
				printf("Exiting...\n");
				break;
			default:
				printf("Not a valid choice!\n");
				break;
		}
	};

	return 0;
}
