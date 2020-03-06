#ifndef BST_H_
#define BST_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

typedef struct node{
	int data;
	uint32_t size;			// Size of subtree, including this node
	struct node *left;
	struct node *right;
} Node;

// Return  0 if inserted, non-zero otherwise
int insert(struct node **, int data);

// Return 0 if removed, non-zero otherwise
int removeNode(struct node **, int data);

// Return non-zero if found, 0 otherwise
int find(const struct node *, int data);

// Return max/min number
int max(const struct node *);
int min(const struct node *);

// Get total number of items in tree
uint32_t size(const struct node *);

uint32_t maxHeight(const struct node *);

// Prints whole tree (in-order)
void printTree(struct node *);

// Free up entire tree
void destroy(struct node **);

#endif
