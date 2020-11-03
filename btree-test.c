#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"btree.h"

int main(int argc, char *argv[]){
	srand(time(0));
	int N = 10;
	struct btree bt = {3, 0, NULL}; // Degree X, size 0, NULL root pointer

	int tmpi;
	if(argc >= 2){
		 tmpi = strtol(argv[1], NULL, 10);
		if(tmpi >= 0) N = tmpi;
	}
	if(argc >= 3){
		tmpi = strtol(argv[2], NULL, 10);
		if(tmpi > 0) bt.degree = tmpi;
	}

	printf("Inserting %7ld data to b-tree with degree %3d\n", N, bt.degree);

	int val, dups = 0;
	int res;
	int *data = malloc(N * sizeof(*data));
	size_t size = 0;
	for(int i = 1;i <= N;i++){
		val = rand() % (8*N);
		//printf("Inserting %3d..\n", val);
		res = btree_insert(&bt, val);
		if(res){
			//printf("Insert FAILED: %d\n", res);
			dups++;
		}else{
			data[size++] = val;
		}
		//btree_print(&bt);
		//printf("after size = %3ld\n", bt.size);
	}

	printf("%d dups\n", dups);
	printf("Final tree of size %d\n", bt.size);
	//btree_print(&bt);

	if((dups + bt.size) != N){
		printf("\nWARNING: Size mismatch. Expected %3d, got %3d\n", N, dups + bt.size);
	}

	// Validate data
	printf("Validating data..\n");
	for(int i = 0;i < size;i++){
		if(!btree_find(&bt, data[i])){
			printf("WARNING: data = %3ld not found!\n", data[i]);
		}
	}

	// Clear
	btree_destroy(&bt);
	free(data);

	return 0;
}
