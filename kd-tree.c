#include <stdio.h>
#include <stdlib.h>

typedef struct KD_Node {
	void *payload;

	struct KD_Node *left;
	struct KD_Node *right;
} kd_node_t;

kd_node_t *node_construct(void *payload) {
	kd_node_t *n_node = malloc(sizeof(kd_node_t));

	n_node->payload = payload;

	n_node->left = NULL;
	n_node->right = NULL;

	return n_node; // :D
}

int node_update_payload(kd_node_t *n_node, void *payload) {
	n_node->payload = payload;

	return 0;
}

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
	kd_node_t *k_node: the node we are trying to fill
		with the pivot
	void **members: the current array of all values
		we will be sorting on and inserting into k_t
	void *dimension: the current dimension we are using
	int low: the current low position in void **members
	int high: the current high position in void **members
*/
int quicksort(kdtree_t *k_t, kd_node_t *k_node, void **members, void *dimension, int low, int high) {
	if (!k_node)
		return 0;

	int pivot = low - 1; // searcher index within members

	void *high_ptr = k_t->member_extract(members[high], dimension);

	for (int j = low; j < high; j++) {
		// if member at j for dimension "dimension",
		// keep moving pivot along
		if (k_t->weight(k_t->member_extract(members[j], dimension), high_ptr))
			swap(members, ++pivot, j);
	}

	// one final swap of pivot and high (so pivot becomes the
	// actual pivot point)
	swap(members, ++pivot, high);

	// shift node payload to match pivot
	node_update_payload(k_node, members[pivot]);

	// recur on both sides:
	k_node->left = low < pivot - 1 ? node_construct(NULL) : NULL;
	k_node->right = pivot + 1 < high ? node_construct(NULL) : NULL;

	dimension = k_t->next_d(dimension);
	// left
	quicksort(k_t, k_node->left, members, dimension, low, pivot - 1);
	// right
	quicksort(k_t, k_node->right, members, dimension, pivot + 1, high);

	return 0; // dun
}

struct KD_Tree {
	int (*weight)(void *, void *); // 0 for "less than",
								   // 1 for "greater than"

	// member extract is used in quicksorting to take in
	// void **members and void *dimension and return
	// what ever the dimension to sort on is for each member
	// within members
	void *(*member_extract)(void *, void *);

	// the array of all the possible dimension, eg.:
	// [x, y] (could be represented as [0, 1] to access another array)
	// [R, G, B]
	void *dimension;
	// will pull whatever dimension occurs next
	// for a linked list, would be the next pointer
	// make sure it reaches a NULL
	void *(*next_d)(void *);

	kd_node_t *kd_head;
};

kdtree_t *new_kdtree(int (*weight)(void *, void *), void *(*member_extract)(void *, void *), void *dimension, void *(*next_d)(void *)) {
	kdtree_t *new_kd = malloc(sizeof(kdtree_t));

	new_kd->weight = weight;

	new_kd->member_extract = member_extract;
	new_kd->next_d = next_d;
	new_kd->dimension = dimension;

	new_kd->kd_head = NULL;

	return new_kd;
}

// this loads the entire tree (and assumes k_t is empty)
int kdtree_load(kdtree_t *k_t, void **members, int member_length) {
	k_t->k_head = node_construct(NULL);
	quicksort(k_t, k_t->k_head, members, dimension, 0, member_length - 1);

	return 0;
}

// searches through current tree to find position for new
// payload based on dimension
int kdtree_insert_helper(kdtree_t *k_t, kd_node_t *k_node, void *payload, void *dimension) {
	// path: 0 for left subtree, 1 for right subtree
	int path = k_t->weight(k_t->member_extract(k_node->payload, dimension), k_t->member_extract(payload, dimension));

	if (!path && !k_node->left) {
		k_node->left = node_construct(payload);

		return 0;
	} else if (path && !k_node->right) {
		k_node->right = node_construct(payload);

		return 0;
	}

	return kdtree_insert_helper(k_t, path ? k_node->right : k_node->left, payload, k_t->next_d(dimension));
}

int kdtree_insert(kdtree_t *k_t, void *payload) {
	if (!k_t->kd_head) {
		k_t->kd_head = node_construct(payload);

		return 0;
	}

	return kdtree_insert_helper(k_t, k_t->kd_head, payload, k_t->dimension);
}

// searches for min in Dth dimension
void *kdtree_min_helper(kdtree_t *k_t, kd_node_t *k_node, void *dimension, void *D) {
	if (!k_node)
		return NULL;

	// get left small (need either way)
	void *left_small = kdtree_min_helper(k_t, k_node->left, k_t->next_d(dimension), D);

	if (dimension == D) // pointer comparison
		// only search on left side:
		return left_small;

	// otherwise get right_small and compare
	void *right_small = kdtree_min_helper(k_t, k_node->right, k_t->next_d(dimension), D);

	// compare each side in the current dimension to choose smallest
	int size = k_t->weight(k_t->member_extract(left_small, dimension), k_t->member_extract(right_small, dimension));

	// if 0, return left_small
	// if 1, return right_small
	if (size)
		return right_small;
	else
		return left_small;
}

void *kdtree_min(kdtree_t *k_t, void *D) {
	if (!k_t->kd_head) return NULL;
	return kdtree_min_helper(k_t, k_t->kd_head, k_t->dimension, D);
}