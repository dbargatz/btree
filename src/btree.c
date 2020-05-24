#include "btree.h"
#include <stdlib.h>

btree_t * btree_create(uint16_t inOrder) {
    if(inOrder < BTREE_MIN_ORDER || inOrder > BTREE_MAX_ORDER) {
        return NULL;
    }

    node_t * root = node_create(inOrder);
    btree_t * tree = malloc(sizeof(btree_t));
    tree->height = 0;
    tree->order = inOrder;
    tree->root = root;
    return tree;
}

void btree_destroy(btree_t * inTree) {
    if(NULL == inTree) {
        return;
    }
    node_destroy(inTree->root);
    free(inTree);
}

void btree_insert(btree_t * inTree, uint64_t inKey, uint64_t inValue) {
    node_t * root;
    if(node_is_full(inTree->root)) {
        root = node_create(inTree->order);
        root->children[0] = inTree->root;
        inTree->root = root;
        inTree->height++;
        node_split_child(root, 0);
        node_insert(root, inKey, inValue);
    } else {
        node_insert(inTree->root, inKey, inValue);
    }
}

uint64_t btree_search(const btree_t * inTree, uint64_t inKey) {
    if(NULL == inTree || INVALID_KEY_VALUE_SENTINEL == inKey) {
        return INVALID_KEY_VALUE_SENTINEL;
    }

    search_result_t result = node_search(inTree->root, inKey);
    if(NULL != result.node) {
        return result.node->keys[result.valueIndex];
    }

    return INVALID_KEY_VALUE_SENTINEL;
}