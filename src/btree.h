#ifndef _BTREE_H
#define _BTREE_H

#include <stdbool.h>  // for bool leaf in node_t
#include <stdint.h>   // for uint types

#define INVALID_SENTINEL (UINT64_MAX)     //< Reserved number for invalid keys/values, cannot insert
#define MIN_DEGREE_MIN   (2)              //< Smallest allowable minimum degree (t) in a B-tree
#define MIN_DEGREE_MAX   (UINT16_MAX / 2) //< Largest allowable minimum degree (t) in a B-tree;
                                          //< constrains max number of keys to UINT16_MAX-1 to
                                          //< fit in uint16_t

/**
 * @brief Individual node of B-tree. Contains keys, values, and children (if
 * any).
 */
typedef struct _node {
    bool leaf;          //< True if node has no children; false if internal node
    uint16_t n;         //< Number of keys. If leaf, t-1 <= n <= 2t-1
    uint16_t t;         //< Minimum degree; 2 <= t <= 32,767 (UINT16_MAX / 2)
    uint64_t * k;       //< `n` keys sorted in ascending order, no duplicates
    uint64_t * v;       //< `n` values; v[i] is value for key k[i]
    struct _node ** c;  //< `n+1` children; all keys c[i]->k < k[i], all keys c[i+1]->k > k[i]
} node_t;

/**
 * @brief B-tree based on CLRS Third Edition, Chapter 18. Stores key/value pairs
 * with guaranteed O(log n) delete, insert, and search. Use provided functions
 * to manipulate.
 * 
 * @see btree_create()
 * @see btree_delete()
 * @see btree_destroy()
 * @see btree_insert()
 * @see btree_search()
 */
typedef struct _btree {
    uint16_t t;  //< Minimum degree; 2 <= t <= 32,767 (UINT16_MAX / 2)
    node_t * r;  //< Root node of B-tree
} btree_t;

/**
 * @brief Result of search; retrieve value with `result.x->v[result.i]`. Node
 * is NULL if key not found.
 */
typedef struct _btree_search_result {
    const node_t * x;  //< Node containing found key; NULL if key not found
    uint16_t i;        //< Index of value in x->v; 0 if key not found
} btree_search_result_t;

/**
 * @brief Allocates contiguous space for an internal B-tree node; exposed for
 * testing purposes only. Not for normal use.
 * 
 * @param t minimum degree of the B-tree
 * @return node_t* allocated node; dispose of with free()
 */
node_t * allocate_node(uint16_t t);

/**
 * @brief Allocates a B-tree of minimum degree `t` and root node.
 * 
 * @param t minimum degree of the B-tree; 2 <= t <= 32,767 (UINT16_MAX / 2)
 * @return btree_t* allocated B-tree; dispose of with btree_destroy()
 * 
 * @see btree_destroy()
 */
btree_t * btree_create(uint16_t t);

/**
 * @brief Removes the given key and associated value from the B-tree, if 
 * present, and re-balances the B-tree in O(log n) time (if necessary). Does
 * nothing if key is not present or is INVALID_SENTINEL.
 * 
 * @param T B-tree to remove from
 * @param k Key to remove; associated value will be removed as well
 * 
 * @see btree_insert()
 */
void btree_delete(btree_t * T, uint64_t k);

/**
 * @brief Recursively destroys a B-tree and frees associated memory.
 * 
 * @param T allocated B-tree to destroy
 * 
 * @see btree_create()
 */
void btree_destroy(btree_t * T);

/**
 * @brief Inserts a given key/value pair into a B-tree, re-balancing the B-tree
 * in O(log n) time (if necessary). If the key is already present, the new value
 * overwrites the old value. Fails if key or value is INVALID_SENTINEL.
 * 
 * @param T B-tree to insert into
 * @param k Key to insert
 * @param v Value to insert, associated with key `k`
 * 
 * @see btree_delete()
 * @see INVALID_SENTINEL
 */
void btree_insert(btree_t * T, uint64_t k, uint64_t v);

/**
 * @brief Finds the node and index of the given key in the B-tree. The value
 * can be retrieved with `result->x->v[result->i]` if the result is valid.
 * 
 * @param x Root node of the B-tree to search
 * @param k Key to find
 * @return btree_search_result_t Search result; { NULL, 0 } if the key wasn't
 * found, or a valid node pointer and key index if the key was found
 */
btree_search_result_t btree_search(const node_t * x, uint64_t k);

#endif // _BTREE_H