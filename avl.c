/*
	avl.c -- AVL tree that stores a single piece of data

	Copyright (C) 2020 Christopher Skane

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

#include"avl.h"

/**	Calculate height of given node
	(Can return size_t, but nothing currently uses return)
**/
static inline void updateHeight(struct node *root){
	//if(root == NULL) return 0; // NEVER happens, as far as I have tested (assuming user doesn't touch AVL structure)

	size_t left = (root->left != NULL) ? left = root->left->height : 0;
	size_t right = (root->right != NULL) ? right = root->right->height : 0;

	root->height = ((left > right) ? left : right) + 1;

	//return root->height;
}

void rotateLeft(struct node **parent, struct node *child){
	//printf("Rotating left with 0x%X and 0x%X\n", *parent, child);
	//if(child == NULL) printf("Something wrong\n");
	// Child (right) replaces parent node
	// Parent becomes left subtree of child
	// Left subtree of child becomes right subtree of parent

	size_t child_size = child->size;
	child->size = (*parent)->size;
	(*parent)->size -= child_size; // Parent losing all nodes of subtree

	struct node *oldChild = child->left;
	if(oldChild) (*parent)->size += oldChild->size;

	child->left = *parent;
	(*parent)->right = oldChild;
	updateHeight(*parent);

	*parent = child;
	updateHeight(child);
}
void rotateRight(struct node **parent, struct node *child){
	//printf("Rotating right with 0x%X and 0x%X\n", *parent, child);
	//if(child == NULL) printf("Something wrong\n");
	// Child (left) replaces parent node
	// Parent becomes right subtree of child
	// Right subtree of child becomes left subtree of parent

	size_t child_size = child->size;
	child->size = (*parent)->size;
	(*parent)->size -= child_size; // Parent losing all nodes of subtree

	struct node *oldChild = child->right;
	if(oldChild) (*parent)->size += oldChild->size;

	child->right = *parent;
	(*parent)->left = oldChild;
	updateHeight(*parent);

	*parent = child;
	updateHeight(child);
}

/**	Rotation helper that will perform the correct rotation for the given root.
	Returns a non-zero value when a rotation occured
**/
int rotate(struct node **tree){
	if(tree == NULL || (*tree) == NULL) return 1;

	size_t left = ((*tree)->left == NULL) ? 0 : (*tree)->left->height;
	size_t right = ((*tree)->right == NULL) ? 0 : (*tree)->right->height;
	int64_t diff = left-right;
	size_t rightChild;
	size_t leftChild;
	int64_t childDiff;
	int ret = 0;

	//if(diff == 2){
	switch(diff){
		case 2:
		// Check for double rotation condition
		// The end root will be the subtree of the child with larger height
		//printf("Checking double condition on left\n");
		rightChild = ((*tree)->left->right == NULL) ? 0 : (*tree)->left->right->height;
		leftChild = ((*tree)->left->left == NULL) ? 0 : (*tree)->left->left->height;
		childDiff = rightChild - leftChild;

		if(childDiff > 0){
			// Rotate left-right
			// Rotate left child and left->right child
			//printf("Left-Right rotation with 0x%X\t0x%X\t0x%X\n", *tree, (*tree)->left, (*tree)->left->right);
			rotateLeft(&((*tree)->left), (*tree)->left->right);
		}

		// Normal rotation right
		rotateRight(tree, (*tree)->left);

		ret = 1; // Rotation occured
	break;
	//}else if(diff == -2){
		case -2:
		//printf("Checking double condition on right\n");
		rightChild = ((*tree)->right->right == NULL) ? 0 : (*tree)->right->right->height;
		leftChild = ((*tree)->right->left == NULL) ? 0 : (*tree)->right->left->height;
		childDiff = leftChild - rightChild;

		if(childDiff > 0){
			// Rotate right-left
			// Rotate right child and right->left child
			//printf("Right-Left rotation with 0x%X\t0x%X\t0x%X\n", *tree, (*tree)->right, (*tree)->right->left);
			rotateRight(&((*tree)->right), (*tree)->right->left);
		}

		// Normal rotation left
		rotateLeft(tree, (*tree)->right);

		ret = 1; // Rotation occured
	break;
	}

	return ret;
}


/**	Inserts data into tree, performs rotations and updates heights
**/
int avlInsert(struct node **tree, data_t data){
	int ret = -1;
	if(tree == NULL) return ret;

#ifndef INSERT_ITER
	/* Typical recursive variant
	*/
	if(*tree == NULL){
		//printf("Inserting %d\n", data);
		(*tree) = malloc(sizeof(Node));// Once at end of branch, insert leaf
		(*tree)->data = data;
		(*tree)->size = 1;
		(*tree)->height = 1;
		(*tree)->left = NULL;
		(*tree)->right = NULL;

		ret = 0;
	}else{
		if((*tree)->data == data){
			ret = -1; // If data already exists
		}else if((*tree)->data > data){
			ret = avlInsert(&((*tree)->left), data); // Recurse to add
		}else if((*tree)->data < data){
			ret = avlInsert(&((*tree)->right), data);
		}

		// Update node data on successful insert
		if(!ret){
			(*tree)->size++; // Increment only if data did not exist
			updateHeight(*tree);

			// Re-balance
			rotate(tree);
		}
	}
#else
	/* Iterative variant
	*/
	ret = 0;
	struct node **root = tree; // Traversal pointer
	struct node **path[64]; // Binary tree max height 64 for 64-bit (probably only need 63)
	int cnt = 0;

	while(*root != NULL && cnt < 64){
		path[cnt++] = root; // Add to path

		if((*root)->data > data){
			root = &(*root)->left;
		}else if((*root)->data < data){
			root = &(*root)->right;
		}else{
			return -1; // Duplicate value, so return early
		}
	}

	// Insert the node
	(*root) = malloc(sizeof(Node));
	(*root)->data = data;
	(*root)->size = 1;
	(*root)->height = 1;
	(*root)->left = NULL;
	(*root)->right = NULL;

	// Reverse path to update height and rotate
	while(--cnt >= 0){
		(*path[cnt])->size++;
		updateHeight(*path[cnt]);

		if(rotate(path[cnt])) break;
	}

	// Update remaining heights and sizes
	while(--cnt >= 0){
		(*path[cnt])->size++;
		updateHeight(*path[cnt]);
	}
#endif

	return ret;
}

data_t avlDeleteMin(struct node **tree){
	if((*tree) != NULL){
		if((*tree)->left != NULL){
			data_t ret = avlDeleteMin(&((*tree)->left));

			(*tree)->size--;
			updateHeight(*tree);

			// Re-balance
			rotate(tree);

			return ret;
		}else{ // Remove min, only has right child
			//printf("Min at 0x%X, moving 0x%X up\n", *tree, (*tree)->right);
			struct node *old = (*tree);
			(*tree) = (*tree)->right; // Move subtree up
			if(*tree != NULL){
				//(*tree)->size--;
				updateHeight((*tree));
				rotate(tree);
			}

			data_t ret = old->data;
			free(old);
			old = NULL;

			return ret;
		}
	}

	return 0;
}
data_t avlDeleteMax(struct node **tree){
	if((*tree) != NULL){
		if((*tree)->right != NULL){
			data_t ret = avlDeleteMax(&((*tree)->right));
			// Only update on successful remove
			(*tree)->size--;
			updateHeight(*tree);

			// Re-balance
			rotate(tree);

			return ret;
		}else{ // Remove max, only has left child
			//printf("Max at 0x%X, moving 0x%X up\n", *tree, (*tree)->left);
			struct node *old = (*tree);
			(*tree) = (*tree)->left; // Move subtree up
			if(*tree != NULL){
				//(*tree)->size--;
				updateHeight((*tree));
				rotate(tree);
			}

			data_t ret = old->data;
			free(old);
			old = NULL;

			return ret;
		}
	}

	return -1;
}

int avlRemove(struct node **tree, data_t data){
	if((*tree) != NULL){
		int ret = 0;
		if((*tree)->data == data){
			// If at the node to remove, get min of right or max of left for root
			// Maybe have it choose larger side to take from
			if((*tree)->right != NULL){
				// Get min value from right subtree to replace root
				(*tree)->data = avlDeleteMin(&((*tree)->right));
			}else if((*tree)->left != NULL){
				// Get max value from left subtree to replace root
				(*tree)->data = avlDeleteMax(&((*tree)->left));
			}else{
				// Node is leaf, simply delete
				free(*tree);
				(*tree) = NULL;
				return 0;
			}

			ret = 0;
		}else if((*tree)->data > data){ // Traversal conditions
			ret = avlRemove(&((*tree)->left), data);
		}else if((*tree)->data < data){
			ret = avlRemove(&((*tree)->right), data);
		}

		// Only update on successful remove
		if(!ret){
			(*tree)->size--;
			updateHeight(*tree);
			rotate(tree);
		}

		return ret;
	}

	return -1;
}

// Returns non-zero if data is in tree, zero otherwise
int avlFind(struct node *tree, data_t data, data_t **value){
	printf("%p\n", value);
	if(tree != NULL){
		if(tree->data == data){
			*value = &tree->data;
			return 1; // Found data
		}
		else if(tree->data > data) return avlFind(tree->left, data, value); // Recurse to add
		else if(tree->data < data) return avlFind(tree->right, data, value);
	}

	return 0;// Not found (false)
}

size_t size(const struct node *tree){
	if(tree != NULL){
		size_t count = size(tree->left);
		count++; // This node
		count += size(tree->right);

		return count;
	}

	return 0;
}

size_t maxHeight(const struct node *root){
	if(root == NULL){
		return 0;
	}

	int left = 0;
	if(root->left != NULL){
		left = maxHeight(root->left) + 1;
	}

	int right = 0;
	if(root->right != NULL){
		right = maxHeight(root->right) + 1;
	}

	if(left > right) return left;
	else return right;

	return 0;
}

data_t max(const struct node *tree){
	if(tree == NULL) return 0;
	if(tree->right == NULL) return tree->data;// If next node is null, max
	return max(tree->right);
}
data_t min(const struct node *tree){
	if(tree == NULL) return 0;
	if(tree->left == NULL) return tree->data;// If next node is null, min
	return min(tree->left);
}

void printTree(struct node *tree){
	if(tree != NULL){
		printTree(tree->left);
		printf("0x%016lX: %5ld\tL: %8X\tR: %8X\t", tree, tree->data, tree->left, tree->right);
		printf("%d/%d\t", (tree->left == NULL) ? 0 : tree->left->size, (tree->right == NULL) ? 0 : tree->right->size);
		printf("%d/%d\n", (tree->left == NULL) ? 0 : tree->left->height, (tree->right == NULL) ? 0 : tree->right->height);
		printTree(tree->right);
	}
}

void destroy(struct node **tree){
	if((*tree) != NULL){
		if((*tree)->left != NULL) destroy(&((*tree)->left));
		if((*tree)->right != NULL) destroy(&((*tree)->right));
		free(*tree);
		*tree = NULL;
	}
}
