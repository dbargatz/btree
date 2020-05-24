#include "btree.h"
#include <stdlib.h>
#include <stdio.h>
#include "../subprojects/munit/munit.h"

btree_t * btree_create(uint16_t t) {
    if(t < DEGREE_MIN || (2*t)-1 > DEGREE_MAX) {
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

void temp_dump(const char * inPrefix, uint64_t inKey, node_t ** inNodes, int inNumNodes, int inLevel) {
    int i, j;
    int k = 0;
    node_t * children[1000];

    if(0 == inLevel) {
        printf("%s (0x%016" PRIX64 ")\n", inPrefix, inKey);
    }
    printf("\tLevel %02d: ", inLevel);
    for(i = 0; i < inNumNodes; i++) {
        printf("[");
        for(j = 0; j < inNodes[i]->n; j++) {
            printf(" 0x%016" PRIX64 "", inNodes[i]->k[j]);
            if(NULL != inNodes[i]->c[j]) {
                children[k++] = inNodes[i]->c[j];
            }
        }
        printf(" ] ");
        if(NULL != inNodes[i]->c[inNodes[i]->n]) {
            children[k++] = inNodes[i]->c[inNodes[i]->n];
        }
    }
    printf("\n");

    if(k > 0) {
        temp_dump(inPrefix, inKey, children, k, inLevel+1);
    }
}

void btree_insert(btree_t * inTree, uint64_t inKey, uint64_t inValue) {
    int t = inTree->t;
    node_t * root = inTree->r;

    if(root->n >= (2 * t)-1) {
        root = node_create(inTree->t);
        root->leaf = false;
        root->n = 0;
        root->c[0] = inTree->r;
        inTree->r = root;
        node_split_child(root, 0);
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