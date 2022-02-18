#ifndef __KD_TREE_L__
#define __KD_TREE_L__

typedef struct KD_Tree kdtree_t;

kdtree_t *new_kdtree(int (*weight)(void *, void *), void *(*member_extract)(void *, void *), void *dimension, void *(*next_d)(void *));

int kdtree_load(kdtree_t *k_t, void **members, int member_length);
int kdtree_insert(kdtree_t *k_t, void *payload);

void *kdtree_min(kdtree_t *k_t);

#endif