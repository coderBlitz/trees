/*
	bst.c -- A binary search tree that stores a single piece of data

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

#include"bst.h"

// This may not actually return parent, but node instead. Check
struct node **getParent(struct node *tree, int data){
	if(tree != NULL){
		if(tree->data > data){
			if(tree->left != NULL){
				if(tree->left->data == data){
					//printf("0x%X has data child 0x%X\n", tree, tree->left);
					return &(tree->left);
				}else return getParent(tree->left, data);
			}
		}else if(tree->data < data){
			if(tree->right != NULL){
				if(tree->right->data == data){
					//printf("0x%X has data child 0x%X\n", tree, tree->right);
					return &(tree->right);
				}else return getParent(tree->right, data);
			}
		}
	}

	return NULL; // Not found (false)
}

// Decrement all sizes up to, but not including node with data
void decrementChain(struct node *root, int data){
	if(root != NULL){
		if(root->data == data){
			return; // Found data
		}
		else if(root->data > data){
			root->size--;
			decrementChain(root->left, data);
		}else if(root->data < data){
			root->size--;
			decrementChain(root->right, data);
		}
	}
}

int insert(struct node **tree, int data){
	if(*tree == NULL){
		//printf("Inserting %d\n", data);
		(*tree) = malloc(sizeof(Node));// Onf at end of branch, insert leaf
		(*tree)->data = data;
		(*tree)->size = 1;
		(*tree)->left = NULL;
		(*tree)->right = NULL;

		return 0;
	}else{
		char ret = 0;
		if((*tree)->data == data){
			return -1; // If data already exists
		}
		else if((*tree)->data > data){
			ret = insert(&((*tree)->left), data); // Recurse to add
			if(!ret) (*tree)->size++; // Increment only if data did not exist
			return ret;
		}else if((*tree)->data < data){
			ret = insert(&((*tree)->right), data);
			if(!ret) (*tree)->size++;
			return ret;
		}
	}

	return -1;
}

int removeNode(struct node **tree, int data){
	if((*tree) == NULL) return -1;

	if((*tree)->data == data){
		// If at the node to remove, get min of right or max of left for root
		// Maybe have it choose larger side to take from
		if((*tree)->right != NULL){
			// Get min value from right subtree to replace root
			(*tree)->data = min((*tree)->right);

			// Either short replace and simply move entire subtree up, or standard.
			if((*tree)->data == (*tree)->right->data){
				struct node *old = (*tree)->right;
				(*tree)->right = (*tree)->right->right; // Move subtree up

				free(old);
				old = NULL;
			}else{
				// Get the node who's value was copied
				struct node **replacement = getParent((*tree)->right, (*tree)->data);
				if(replacement == NULL){
					printf("Remove failed\n");
					return -1;
				}

				decrementChain((*tree)->right, (*tree)->data);
				struct node *old = (*replacement);
				(*replacement) = (*replacement)->right; // Replace moved node with subtree
				free(old);
			}

			(*tree)->size--;
		}else if((*tree)->left != NULL){
			// Get max value from left subtree to replace root
			(*tree)->data = max((*tree)->left);

			// Same replace decision as above, with left subtree instead
			if((*tree)->data == (*tree)->left->data){
				struct node *old = (*tree)->left;
				(*tree)->left = (*tree)->left->left; // Move subtree up

				free(old);
				old = NULL;
			}else{
				struct node **replacement = getParent((*tree)->left, (*tree)->data);
				if(replacement == NULL){
					printf("Remove failed\n");
					return -1;
				}

				decrementChain((*tree)->left, (*tree)->data);
				struct node *old = (*replacement);
				(*replacement) = (*replacement)->left; // Replace moved node with subtree
				free(old);
			}

			(*tree)->size--;
		}else{
			// Node is leaf, simply delete
			free(*tree);
			(*tree) = NULL;
		}

		return 0;
	}
	else if((*tree)->data > data){ // Traversal conditions
		int ret = removeNode(&((*tree)->left), data);
		if(!ret) (*tree)->size--;
		return ret;
	}else if((*tree)->data < data){
		int ret = removeNode(&((*tree)->right), data);
		if(!ret) (*tree)->size--;
		return ret;
	}

	return -1;
}

// Returns non-zero if data is in tree, zero otherwise
int find(const struct node *tree, int data){
	if(tree != NULL){
		if(tree->data == data){
			return 1; // Found data
		}
		else if(tree->data > data) return find(tree->left, data); // Recurse to add
		else if(tree->data < data) return find(tree->right, data);
	}

	return 0;// Not found (false)
}

uint32_t size(const struct node *tree){
	if(tree != NULL){
		int count = size(tree->left);
		count++; // This node
		count += size(tree->right);

		return count;
	}

	return 0;
}

uint32_t maxHeight(const struct node *root){
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

int max(const struct node *tree){
	if(tree == NULL) return 0;
	if(tree->right == NULL) return tree->data;// If next node is null, max
	return max(tree->right);
}
int min(const struct node *tree){
	if(tree == NULL) return 0;
	if(tree->left == NULL) return tree->data;// If next node is null, min
	return min(tree->left);
}

void printTree(struct node *tree){
	if(tree != NULL){
		printTree(tree->left);
		printf("0x%08X: %d\tL: %8X\tR: %8X\t", tree, tree->data, tree->left, tree->right);
		printf("%d/%d\n", (tree->left == NULL)?0:tree->left->size, (tree->right == NULL)?0:tree->right->size);
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
