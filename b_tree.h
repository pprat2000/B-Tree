#ifndef __B_TREE__
#define __B_TREE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Just Debug Things
#define DEBUG	0x1

#ifdef DEBUG
#define DBG_PRINT(...) fprintf( stdout, __VA_ARGS__ )
#else
#define DBG_PRINT(...) do{ } while ( 0 )
#endif


#define deg     0x2
// B Tree Node struct parameterized
// by the above #define
struct b_tree_node {
	int cnt;
	int key[2*deg - 1];
	struct b_tree_node *pptr;
	struct b_tree_node *child[2*deg];
};
typedef struct b_tree_node bt_node;

void insert_to_tree(int val, bt_node **node);
void split_and_insert(int val, bt_node *node);
#endif // __B_TREE__