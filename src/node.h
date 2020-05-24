#ifndef _NODE_H
#define _NODE_H

#include <stdbool.h>
#include <stdint.h>

#define INVALID_CHILD_SENTINEL     (UINT64_MAX)
#define INVALID_KEY_VALUE_SENTINEL (UINT64_MAX)

typedef struct _node {
    uint16_t numKeys;
    uint16_t order;
    uint64_t * keys;
    uint64_t * values;
    struct _node ** children;
} node_t;

typedef struct _search_result {
    struct _node * node;
    uint16_t valueIndex;
} search_result_t;

node_t * node_create(uint16_t inOrder);
void node_destroy(node_t * inNode);
void node_insert(node_t * inParent, uint64_t inKey, uint64_t inValue);
bool node_is_full(node_t * inNode);
bool node_is_leaf(node_t * inNode);
search_result_t node_search(node_t * inNode, uint64_t inKey);
void node_split_child(node_t * inParent, uint16_t inSplitIndex);


#endif // _NODE_H