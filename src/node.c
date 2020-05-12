#include "node.h"
#include <stdlib.h>
#include <stdio.h> // TODO: remove

node_t * node_create(uint16_t inOrder) {
    node_t * node;
    uint16_t keyLength = sizeof(*node->keys) * (inOrder-1);
    uint16_t valLength = sizeof(*node->values) * (inOrder-1);
    uint16_t childrenLength = sizeof(*node->children) * inOrder;
    node = malloc(sizeof(node_t) + keyLength + valLength + childrenLength);

    node->numKeys = 0;
    node->order = inOrder;
    node->keys = (uint64_t *)(((uint8_t *)node) + sizeof(node_t));
    node->values = (uint64_t *)(((uint8_t *)node->keys) + keyLength);
    node->children = (node_t **)(((uint8_t *)node->values) + valLength);
    node->children[0] = NULL;
    return node;
}

void node_destroy(node_t * inNode) {
    int i;
    if(!node_is_leaf(inNode)) {
        for(i = 0; i <= inNode->numKeys; i++) {
            node_destroy(inNode->children[i]);
            inNode->children[i] = NULL;
        }
    }
    free(inNode);
}

void node_insert(node_t * inNode, uint64_t inKey, uint64_t inValue) {
    int i = inNode->numKeys - 1;

    if(node_is_leaf(inNode)) {
        printf("node_insert leaf\n");
        while(i >= 0 && inKey < inNode->keys[i]) {
            inNode->keys[i+1] = inNode->keys[i];
            i--;
        }
        i++;
        inNode->keys[i] = inKey;
        inNode->values[i] = inValue;
        inNode->numKeys++;
    } else {
        printf("node_insert internal\n");
        while(i >= 0 && inKey < inNode->keys[i]) {
            i--;
        }
        i++;
        if(node_is_full(inNode->children[i])) {
            node_split_child(inNode, i);
            if(inKey > inNode->keys[i]) {
                i++;
            }
        }
        node_insert(inNode->children[i], inKey, inValue);
    }
}

bool node_is_full(node_t * inNode) {
    return (inNode->numKeys >= (inNode->order-1));
}

bool node_is_leaf(node_t * inNode) {
    return (NULL == inNode->children[0]);
}

search_result_t node_search(node_t * inNode, uint64_t inKey) {
    int i;
    search_result_t result;

    for(i = 0; i < inNode->numKeys; i++) {
        if(inKey == inNode->keys[i]) {
            result.node = inNode;
            result.valueIndex = i;
            return result;
        } else if(inKey > inNode->keys[i]) {
            break;
        }
    }

    // If this is a leaf node, return an empty result.
    if(NULL == inNode->children[0]) {
        result.node = NULL;
        result.valueIndex = 0;
        return result;
    } else {
        return node_search(inNode->children[i], inKey);
    }
}

void node_split_child(node_t * inParent, uint16_t inSplitIndex) {
    printf("TODO: Split not implemented.\n");
}