/*
	avl-test.c -- A testing program for the AVL tree.

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

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"avl.h"

int checkAVL(Node *root){
	if(root == NULL){
		printf("Empty root\n");
		return 0;
	}

	int good = 0;

	if(root->left == NULL && root->right == NULL){
		/*if(root->size != 1){
			printf("Leaf 0x%X has invalid size of %d\n", root->size);
			good = -1;
		}*/
	}

	if(root->left != NULL){ // Check left subtree
		/*if(root->left->size != (root->size-1)){
			printf("Left 0x%X size is %d, should be %d\n", root->left, root->left->size, root->size-1);
			good = -1;
		}*/

		if(root->left->data > root->data){
			printf("Left 0x%X has larger data than 0x%X. %d !< %d\n", root->left, root, root->left->data, root->data);
			good = -1;
		}else if(root->left->data == root->data){
			printf("Left 0x%X has duplicate data of 0x%X. %d\n", root->left, root, root->data);
			good = -1;
		}

		good = checkAVL(root->left) || good;
	}

	if(root->right != NULL){ // Check right subtree
		/*if(root->right->size != (root->size-1)){
			printf("Right 0x%X size is %d, should be %d\n", root->right, root->right->size, root->size-1);
			good = -1;
		}*/

		if(root->right->data < root->data){
			printf("Right 0x%X has smaller data than 0x%X. %d !< %d\n", root->right, root, root->right->data, root->data);
			good = -1;
		}else if(root->right->data == root->data){
			printf("Right 0x%X has duplicate data of 0x%X. %d\n", root->right, root, root->data);
			good = -1;
		}

		good = checkAVL(root->right) || good;
	}

	if(root->left != NULL && root->right != NULL){
		// Both children non-null, check size
		int left = (root->left == NULL) ? 0 : root->left->size;
		int right = (root->right == NULL) ? 0 : root->right->size;
		int combo = left + right;
		if(combo != (root->size - 1)){
			printf("0x%X size %d != %d + %d + 1 = %d\n", root, root->size, left, right, combo+1);
			good = -1;
		}
	}

	uint32_t left = (root->left == NULL)?0:root->left->height;
	uint32_t right = (root->right == NULL)?0:root->right->height;
	int32_t diff = left - right;
	if(diff > 1 || diff < -1){
		printf("0x%X has bad subtree heights, 0x%X = %u\t0x%X = %u\n", root, root->left, left, root->right, right);
		good = -1;
	}

	return good;
}

int main(int argc, char *argv[]){
	srand(time(0));

	Node *test = NULL;
	int N = 100000;
	if(argc == 2){
		N = strtol(argv[1], NULL, 10);
	}

	int mod = N*3;
	long nums[N];

	printf("Inserting %d numbers into test..\n",N);
	for(int i = 0;i < N;i++){
		nums[i] = rand() % mod;
		//printf("Inserting %d..\n", nums[i]);
		if(avlInsert(&test, nums[i])){
			//printf("%d is a duplicate\n", nums[i]);
			--i;
			continue;
		}else if(checkAVL(test)){
			printf("Tree failed check on iteration %d, after inserting %d\n", i, nums[i]);
			printTree(test);
			break;
		}
	}

	//printf("\nPrinting test(0x%X)..\n",test);
	//printTree(test);

	printf("\nChecking tree after insert..\n");
	int ret = checkAVL(test);
	printf("Tree is %s\n\n", (ret)?"bad":"good");

	//printf("\nTrying to remove root node test(%d)..\n", test->data);
	printf("Trying to remove everything..\n");
	for(int i = 0;i < N;i++){
		//printf("Removing %d\n", nums[i]);
		if(avlRemove(&test, nums[i])){
			printf("Failed to remove %d\n", nums[i]);
			break;
		}else if(checkAVL(test)){ // Adds considerable time to removals
			printf("Tree failed check on iteration %d, after removing %d\n", i, nums[i]);
			printTree(test);
			break;
		}
	}

	printf("Destroying..\n");
	destroy(&test);
}
