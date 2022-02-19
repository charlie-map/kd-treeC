#include <stdio.h>
#include <stdlib.h>

#include "kd-tree.h"

int test() {
	kdtree_t *k_t1 = kdtree_create(default_weight, default_member_extract, default_next_d);

	kdtree_destroy(k_t1);

	return 0;
}

int main() {
	test();

	printf("\nALL TESTS PASSED\n");

	return 0;
}