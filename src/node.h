#ifndef _NODE_H
#define _NODE_H

#include <stdbool.h>
#include <stdint.h>

#define DEGREE_MIN     (2)
#define DEGREE_MAX     (UINT16_MAX / 2)
#define INVALID_KEY_VALUE_SENTINEL (UINT64_MAX)

/**
 * @brief B-tree node with `n` keys, minimum degree `t`, `n + 1` children (if
 * internal node).
 * 
 */
typedef struct _node {
    bool leaf;           //< true if node has no children; false if internal node
    uint16_t n;          //< number of keys
    uint16_t t;          //< min degree; `n` must be t-1 <= n <= 2t-1 unless node is root
    uint64_t * k;        //< array of keys
    uint64_t * values;   //< array of values; values[i] associated with k[i]
    struct _node ** c;   //< array of children; if internal node, array is empty
} node_t;

/**
 * @brief Result of search; retrieve value with `node->values[valueIndex]`. Node
 * is NULL if key not found.
 * 
 */
typedef struct _search_result {
    struct _node * node;    //< node containing found key; NULL if key not found
    uint16_t valueIndex;    //< index of value in node->values; 0 if key not found
} search_result_t;

node_t * node_create(uint16_t t);
void node_destroy(node_t * x);
void node_insert(node_t * x, uint64_t k, uint64_t inValue);
search_result_t node_search(node_t * x, uint64_t k);
void node_split_child(node_t * x, uint16_t i);


#endif // _NODE_H