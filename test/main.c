#include <stdio.h>
#include "../src/btree.h"

void node_dump(node_t * inNode, uint16_t inLevel) {
    int i;
    for(i = 0; i < inLevel; i++) {
        printf("\t");
    }
    printf("[");
    for(i = 0; i < inNode->numKeys; i++) {
        printf(" %02ld ",inNode->keys[i]);
    }
    printf("]");

    if(!node_is_leaf(inNode)) {
        for(i = 0; i <= inNode->numKeys; i++) {
            printf("\n");
            node_dump(inNode->children[i], inLevel+1);
        }
    }
}

void btree_dump(btree_t * inTree) {
    printf("B-Tree (Height %d):\n", inTree->height);
    node_dump(inTree->root, 0);
    printf("\n");
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

    btree_insert(tree, 10, 10);
    btree_dump(tree);

    btree_insert(tree, 1, 1);
    btree_dump(tree);

    btree_insert(tree, 3, 3);
    btree_dump(tree);

    btree_insert(tree, 5, 5);
    btree_dump(tree);

    btree_insert(tree, 7, 7);
    btree_dump(tree);

    btree_insert(tree, 9, 9);
    btree_dump(tree);

    btree_insert(tree, 11, 11);
    btree_dump(tree);

    btree_insert(tree, 12, 12);
    btree_dump(tree);

    btree_insert(tree, 13, 13);
    btree_dump(tree);

    btree_destroy(tree);
    printf("Destroyed tree.\n");
    return 0;
}