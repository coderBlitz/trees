/*
	avl.h -- AVL tree that stores a single piece of data

	Copyright (C) 2019 Christopher Skane

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	Full license at https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
*/

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
