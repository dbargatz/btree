#include "btree.h"
#include <stdlib.h>

btree_t * btree_create(uint16_t t) {
    if(t < DEGREE_MIN || t > DEGREE_MAX) {
        return NULL;
    }

    node_t * x = node_create(t);
    x->leaf = true;
    btree_t * tree = malloc(sizeof(btree_t));
    tree->t = t;
    tree->r = x;
    return tree;
}

void btree_destroy(btree_t * inTree) {
    if(NULL == inTree) {
        return;
    }
    node_destroy(inTree->r);
    free(inTree);
}

void btree_insert(btree_t * inTree, uint64_t inKey, uint64_t inValue) {
    node_t * s;

    if(NULL == inTree || NULL == inTree->r) {
        return;
    }

    if(inTree->r->n >= (2 * inTree->t)-1) {
        s = node_create(inTree->t);
        s->leaf = false;
        s->n = 0;
        s->c[0] = inTree->r;
        inTree->r = s;
        node_split_child(s, 0);
    }
    node_insert(inTree->r, inKey, inValue);
}

uint64_t btree_search(const btree_t * inTree, uint64_t inKey) {
    if(NULL == inTree || INVALID_KEY_VALUE_SENTINEL == inKey) {
        return INVALID_KEY_VALUE_SENTINEL;
    }

    search_result_t result = node_search(inTree->r, inKey);
    if(NULL != result.node) {
        return result.node->k[result.valueIndex];
    }

    return INVALID_KEY_VALUE_SENTINEL;
}