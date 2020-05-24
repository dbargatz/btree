#include "node.h"
#include <stdlib.h>
#include <string.h>

node_t * node_create(uint16_t inOrder) {
    node_t * node;
    uint64_t keyLength = sizeof(*node->keys) * (inOrder-1);
    uint64_t valLength = sizeof(*node->values) * (inOrder-1);
    uint64_t childrenLength = sizeof(*node->children) * inOrder;
    node = malloc(sizeof(node_t) + keyLength + valLength + childrenLength);

    node->numKeys = 0;
    node->order = inOrder;
    node->keys = (uint64_t *)(((uint8_t *)node) + sizeof(node_t));
    node->values = (uint64_t *)(((uint8_t *)node->keys) + keyLength);
    node->children = (node_t **)(((uint8_t *)node->values) + valLength);
    memset(node->children, 0, childrenLength);
    return node;
}

void node_destroy(node_t * inNode) {
    int i;

    if(NULL == inNode) {
        return;
    }

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
        while(i >= 0 && inKey < inNode->keys[i]) {
            inNode->keys[i+1] = inNode->keys[i];
            i--;
        }
        i++;
        inNode->keys[i] = inKey;
        inNode->values[i] = inValue;
        inNode->numKeys++;
    } else {
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
    search_result_t result = { .node = NULL, .valueIndex = 0 };

    if(NULL == inNode || INVALID_KEY_VALUE_SENTINEL == inKey) {
        return result;
    }

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
        return result;
    } else {
        return node_search(inNode->children[i], inKey);
    }
}

void node_split_child(node_t * inParent, uint16_t inSplitIndex) {
    int j;
    int t = (inParent->order / 2);
    node_t * left = inParent->children[inSplitIndex];
    node_t * right = node_create(inParent->order);
    if(node_is_leaf(left)) {
        right->children[0] = NULL;
    }

    right->numKeys = t - 1;
    for(j = 0; j < t - 1; j++) {
        right->keys[j] = left->keys[j + t];
        right->values[j] = left->values[j + t];
    }

    if(!node_is_leaf(left)) {
        for(j = 0; j <= t - 1; j++) {
            right->children[j] = left->children[j + t];
        }
    }

    left->numKeys = t - 1;
    for(j = inParent->numKeys + 1; j > inSplitIndex + 1; j--) {
        inParent->children[j+1] = inParent->children[j];
    }
    inParent->children[inSplitIndex+1] = right;

    for(j = inParent->numKeys; j > inSplitIndex; j--) {
        inParent->keys[j+1] = inParent->keys[j];
        inParent->values[j+1] = inParent->values[j];
    }
    inParent->keys[inSplitIndex] = left->keys[t-1];
    inParent->values[inSplitIndex] = left->values[t-1];
    inParent->numKeys++;
}