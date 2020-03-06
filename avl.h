#ifndef AVL_H_
#define AVL_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

typedef long data_t;

typedef struct node{
	data_t data;
	size_t size;			// Size of subtree, including this node (No longer broken)
	size_t height;
	struct node *left;
	struct node *right;
} Node;

// Return  0 if inserted, non-zero otherwise
int avlInsert(struct node **, data_t data);

// Return 0 if removed, non-zero otherwise
int avlRemove(struct node **, data_t data);

// Return non-zero if found, 0 otherwise
int avlFind(struct node *, data_t data, data_t **val);

// Return max/min number
data_t max(const struct node *);
data_t min(const struct node *);

// Get total number of items in tree
size_t size(const struct node *);

size_t maxHeight(const struct node *);

// Prints whole tree (in-order)
void printTree(struct node *);

// Free up entire tree
void destroy(struct node **);

#endif
