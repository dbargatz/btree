#include "btree.h"
#include <stdlib.h>   // for malloc, free

/**
 * @brief Inserts a key/value pair into a leaf node, recursing through internal
 * nodes and splitting full nodes or overwriting duplicate keys as necessary.
 * Internal function.
 * 
 * @param x Node to attempt insert on, split, and/or recurse through
 * @param k Key to insert/overwrite
 * @param v Value associated with key to insert
 * 
 * @see btree_insert()
 */
void btree_insert_nonfull(node_t * x, uint64_t k, uint64_t v);

/**
 * @brief Splits a full node around the given index and re-balances the local
 * portion of the B-tree. Internal function.
 * 
 * @param x Full node to split
 * @param i Index of median key to split around
 */
void btree_split_child(node_t * x, uint16_t i);

/**
 * @brief Recursively frees a node and its child nodes (if any). Internal
 * function.
 * 
 * @param x Node to destroy
 * 
 * @see allocate_node()
 */
void destroy_node(node_t * x);

/**
 * @brief Sets the __n uint64_t's pointed to by __s to __c. Internal function.
 * 
 * @param __s Pointer to array of __n uint64_ts
 * @param __c Value to set each uint64_t to
 * @param __n Number of uint64_ts to set
 */
void memset64(void * __s, uint64_t __c, size_t __n);

node_t * allocate_node(uint16_t t) {
    node_t * x;
    uint64_t kBytes = sizeof(*x->k) * ((2*t)-1);
    uint64_t vBytes = sizeof(*x->v) * ((2*t)-1);
    uint64_t cBytes = sizeof(*x->c) * (2*t);
    x = malloc(sizeof(*x) + kBytes + vBytes + cBytes);

    x->n = 0;
    x->t = t;

    x->k = (uint64_t *)(((uint8_t *)x) + sizeof(*x));
    memset64(x->k, INVALID_SENTINEL, (2*t)-1);

    x->v = (uint64_t *)(((uint8_t *)x->k) + kBytes);
    memset64(x->v, INVALID_SENTINEL, (2*t)-1);

    x->c = (node_t **)(((uint8_t *)x->v) + vBytes);
    memset64(x->c, (uint64_t)NULL, 2*t);

    return x;
}

btree_t * btree_create(uint16_t t) {
    if(t < MIN_DEGREE_MIN || t > MIN_DEGREE_MAX) {
        return NULL;
    }

    node_t * x = allocate_node(t);
    x->leaf = true;
    btree_t * tree = malloc(sizeof(btree_t));
    tree->t = t;
    tree->r = x;
    return tree;
}

void btree_delete(node_t * x, uint64_t k) {
    if(NULL == x || INVALID_SENTINEL == k) {
        return;
    }

    int i = x->n - 1;
    while(i >= 0 && k < x->k[i]) {
        i--;
    }
    
    // Case 1: k in x->k, x is leaf
    if(i >= 0 && x->k[i] == k && x->leaf) {
        // Remove k/v from x.
        while(i < x->n-1) {
            x->k[i] = x->k[i+1];
            x->v[i] = x->v[i+1];
            i++;
        }
        x->k[i] = INVALID_SENTINEL;
        x->v[i] = INVALID_SENTINEL;
        x->n--;
        return;
    }
}

void btree_destroy(btree_t * T) {
    if(NULL == T) {
        return;
    }
    destroy_node(T->r);
    free(T);
}

void btree_insert(btree_t * T, uint64_t k, uint64_t v) {
    node_t * s;

    if(NULL == T || NULL == T->r || INVALID_SENTINEL == k || INVALID_SENTINEL == v) {
        return;
    }

    if(T->r->n >= (2 * T->t)-1) {
        s = allocate_node(T->t);
        s->leaf = false;
        s->n = 0;
        s->c[0] = T->r;
        T->r = s;
        btree_split_child(s, 0);
        btree_insert_nonfull(s, k, v);
    } else {
        btree_insert_nonfull(T->r, k, v);
    }
}

void btree_insert_nonfull(node_t * x, uint64_t k, uint64_t v) {
    int i = x->n - 1;

    if(x->leaf) {
        while(i >= 0 && k < x->k[i]) {
            i--;
        }

        // Departure from CLRS: duplicate keys overwrite their existing value.
        if(x->k[i] == k) {
            x->v[i] = v;
        } else {
            for(i = x->n-1; i >= 0 && k < x->k[i]; i--) {
                x->k[i+1] = x->k[i];
                x->v[i+1] = x->v[i];
            }
            x->k[i+1] = k;
            x->v[i+1] = v;
            x->n++;
        }
    } else {
        while(i >= 0 && k < x->k[i]) {
            i--;
        }

        // Departure from CLRS: duplicate keys overwrite their existing value.
        if(x->k[i] == k) {
            x->v[i] = v;
        } else {
            i++;
            if(x->c[i]->n == (2*x->t)-1) {
                btree_split_child(x, i);
                if(k > x->k[i]) {
                    i++;
                }
            }
            btree_insert_nonfull(x->c[i], k, v);
        }
    }
}

btree_search_result_t btree_search(const node_t * x, uint64_t k) {
    int i = 0;

    if(NULL == x || INVALID_SENTINEL == k) {
        return (btree_search_result_t){ NULL, 0 };
    }

    while(i < x->n && k > x->k[i]) {
        i++;
    }

    if(i < x->n && k == x->k[i]) {
        return (btree_search_result_t){ x, i };
    } else if(x->leaf) {
        return (btree_search_result_t){ NULL, 0 };
    } else {
        return btree_search(x->c[i], k);
    }
}

void btree_split_child(node_t * x, uint16_t i) {
    int j;
    node_t * z = allocate_node(x->t);
    node_t * y = x->c[i];
    z->leaf = y->leaf;
    z->n = x->t - 1;

    for(j = 0; j < x->t-1; j++) {
        z->k[j] = y->k[j+x->t];
        y->k[j+x->t] = INVALID_SENTINEL;

        z->v[j] = y->v[j+x->t];
        y->v[j+x->t] = INVALID_SENTINEL;
    }

    if(!y->leaf) {
        for(j = 0; j < x->t; j++) {
            z->c[j] = y->c[j+x->t];
            y->c[j+x->t] = NULL;
        }
    }
    y->n = x->t-1;

    for(j = x->n; j > i; j--) {
        x->c[j+1] = x->c[j];
    }
    x->c[i+1] = z;

    for(j = x->n - 1; j >= i; j--) {
        x->k[j+1] = x->k[j];
        x->v[j+1] = x->v[j];
    }
    x->k[i] = y->k[x->t-1];
    x->v[i] = y->v[x->t-1];
    x->n++;
    y->k[x->t-1] = INVALID_SENTINEL;
    y->v[x->t-1] = INVALID_SENTINEL;
}

void destroy_node(node_t * x) {
    int i;

    if(NULL == x) {
        return;
    }

    if(!x->leaf) {
        for(i = 0; i <= x->n; i++) {
            destroy_node(x->c[i]);
            x->c[i] = NULL;
        }
    }
    free(x);
}

void memset64(void * __s, uint64_t __c, size_t __n) {
    size_t i;
    for(i = 0; i < __n; i++) {
        ((uint64_t *)__s)[i] = __c;
    }
}
