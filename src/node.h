#ifndef _NODE_H
#define _NODE_H

#include <stdbool.h>
#include <stdint.h>

#define DEGREE_MIN     (2)
#define DEGREE_MAX     (UINT16_MAX / 2)
#define INVALID_KEY_VALUE_SENTINEL (UINT64_MAX)

typedef struct _node {
    bool leaf;
    uint16_t n;
    uint16_t t;
    uint64_t * k;
    uint64_t * values;
    struct _node ** c;
} node_t;

typedef struct _search_result {
    struct _node * node;
    uint16_t valueIndex;
} search_result_t;

node_t * node_create(uint16_t t);
void node_destroy(node_t * x);
void node_insert(node_t * x, uint64_t k, uint64_t inValue);
search_result_t node_search(node_t * x, uint64_t k);
void node_split_child(node_t * x, uint16_t i);


#endif // _NODE_H