#ifndef _BTREE_H
#define _BTREE_H

#include <stdbool.h>  // for bool leaf in node_t
#include <stdint.h>   // for uint types

#define MIN_DEGREE_MIN   (2)
#define MIN_DEGREE_MAX   (UINT16_MAX / 2)
#define INVALID_SENTINEL (UINT64_MAX)

/**
 * @brief B-tree node with `n` keys, minimum degree `t`, `n + 1` children (if
 * internal node).
 * 
 */
typedef struct _node {
    bool leaf;          //< true if node has no children; false if internal node
    uint16_t n;         //< number of keys
    uint16_t t;         //< min degree; `n` must be t-1 <= n <= 2t-1 unless node is root
    uint64_t * k;       //< array of keys
    uint64_t * v;       //< array of values; values[i] associated with k[i]
    struct _node ** c;  //< array of children; if internal node, array is empty
} node_t;

typedef struct _btree {
    uint16_t t;
    node_t * r;
} btree_t;

/**
 * @brief Result of search; retrieve value with `node->values[valueIndex]`. Node
 * is NULL if key not found.
 * 
 */
typedef struct _btree_search_result {
    const node_t * x;  //< node containing found key; NULL if key not found
    uint16_t i;        //< index of value in node->values; 0 if key not found
} btree_search_result_t;

btree_t * btree_create(uint16_t t);
void btree_delete(btree_t * T, uint64_t k);
void btree_destroy(btree_t * T);
void btree_insert(btree_t * T, uint64_t k, uint64_t v);
btree_search_result_t btree_search(const node_t * x, uint64_t k);

#endif // _BTREE_H