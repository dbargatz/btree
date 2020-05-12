#include <stdio.h>
#include "../src/btree.h"

void node_dump(node_t * inNode) {
    int i;
    printf("[");
    for(i = 0; i < inNode->numKeys; i++) {
        printf(" %02ld ",inNode->keys[i]);
    }
    printf("]\n");

    if(!node_is_leaf(inNode)) {
        for(i = 0; i <= inNode->numKeys; i++) {
            node_dump(inNode->children[i]);
        }
    }
}

void btree_dump(btree_t * inTree) {
    printf("B-Tree (Height %d):\n", inTree->height);
    node_dump(inTree->root);
}

int main(void) {
    printf("Running tests.\n");
    btree_t * tree = btree_create(4);
    printf("Created tree.\n");

    btree_insert(tree, 2, 2);
    btree_dump(tree);

    btree_insert(tree, 4, 4);
    btree_dump(tree);

    btree_insert(tree, 6, 6);
    btree_dump(tree);

    btree_insert(tree, 8, 8);
    btree_dump(tree);

    btree_destroy(tree);
    printf("Destroyed tree.\n");
    return 0;
}