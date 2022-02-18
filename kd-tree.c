#include <stdio.h>
#include <stdlib.h>

typedef struct KD_Node {
	void *payload;

	struct KD_Node *left;
	struct KD_Node *right;
} kd_node_t;

int swap(void **members, int m1, int m2) {
	void *buff = members[m1];
	members[m1] = members[m2];
	members[m2] = buff;

	return 0;
}

/*
	This quicksort implementation takes:

	kdtree_t *k_t: and empty kd-tree that will slowly
		fill as pivot points are found
	void **members: the current array of all values
		we will be sorting on and inserting into k_t
	void *dimension: the current dimension we are using
	int low: the current low position in void **members
	int high: the current high position in void **members
*/
int quicksort(kdtree_t *k_t, void **members, void *dimension, int low, int high) {
	int pivot = low - 1; // searcher index within members

	void *high_ptr = k_t->member_extract(members[high], dimension);

	for (int j = low; j < high; j++) {
		// if member at j for dimension "dimension",
		// keep moving pivot along
		if (k_t->weight(k_t->member_extract(members[j], dimension), high_ptr)) {
			pivot++;
			swap()
		}
	}
}

struct KD_Tree {
	int (*weight)(void *, void *); // 0 for "less than",
								   // 1 for "greater than"

	// member extract is used in quicksorting to take in
	// void **members and void *dimension and return
	// what ever the dimension to sort on is for each member
	// within members
	void *(*member_extract)(void *, void *);

	// will pull whatever dimension occurs next
	// for a linked list, would be the next pointer
	// make sure it reaches a NULL
	void *(*next_d)(void *);

	kd_node_t *kd_head;
};