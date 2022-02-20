#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "kd-tree.h"

int test() {
	int *dimension = malloc(sizeof(int));
	*dimension = 0;
	kdtree_t *k_t1 = kdtree_create(default_weight, default_member_extract, dimension, default_next_d);

	int **member1 = malloc(sizeof(int *) * 2);
	member1[0] = malloc(sizeof(int));
	*(member1[0]) = 20; // x value
	member1[1] = malloc(sizeof(int));
	*(member1[1]) = 10; // y value

	kdtree_insert(k_t1, member1);

	int **member2 = malloc(sizeof(int *) * 2);
	member2[0] = malloc(sizeof(int));
	*(member2[0]) = 30; // x value
	member2[1] = malloc(sizeof(int));
	*(member2[1]) = 5; // y value

	kdtree_insert(k_t1, member2);

	kdtree_destroy(k_t1);

	free(member1[0]);
	free(member1[1]);
	free(member1);

	free(member2[0]);
	free(member2[1]);
	free(member2);


	/* Doing structured load (still x, y) */
	kdtree_t *k_t2 = kdtree_create(default_weight, default_member_extract, dimension, default_next_d);

	int ***members = malloc(sizeof(int **) * 40);

	for (int set_member = 0; set_member < 40; set_member++) {
		members[set_member] = malloc(sizeof(int *) * 2);
		members[set_member][0] = malloc(sizeof(int));
		*(members[set_member][0]) = set_member;
		members[set_member][1] = malloc(sizeof(int));
		*(members[set_member][1]) = 40 - set_member;
	}

	kdtree_load(k_t2, (void ***) members, 40);

	kdtree_destroy(k_t2);

	for (int delete_member = 0; delete_member < 40; delete_member++) {
		free(members[delete_member][0]);
		free(members[delete_member][1]);
		free(members[delete_member]);
	}

	free(members);

	free(dimension);

	return 0;
}

int main() {
	test();

	printf("\nALL TESTS PASSED\n");

	return 0;
}