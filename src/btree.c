#include "btree.h"
#include <stdlib.h>   // for malloc, free

/**
 * @brief Removes the given key and associated value from the subtree rooted
 * at the given node, if present, and re-balances the B-tree in O(log n) time 
 * (if necessary). Does nothing if key is not present or is INVALID_SENTINEL.
 * 
 * @param x Root node of subtree to remove from
 * @param k Key to remove; associated value will be removed as well
 * 
 * @see btree_delete()
 */
void btree_delete_from_subtree(node_t * x, uint64_t k);

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

void btree_delete(btree_t * T, uint64_t k) {
    if(NULL == T || NULL == T->r || INVALID_SENTINEL == k) {
        return;
    }

    node_t * root = T->r;
    btree_delete_from_subtree(root, k);

    // It's possible that the root node is now completely empty. If so,
    // replace the root with its only child and free the old root.
    if(0 == root->n) {
        T->r = root->c[0];
        root->c[0] = NULL;
        destroy_node(root);
    }
}

void btree_delete_from_subtree(node_t * x, uint64_t k) {
    if(NULL == x || INVALID_SENTINEL == k) {
        return;
    }

    int j;
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

    // Case 2: k in x->k, x is internal (not leaf)
    if(i >= 0 && x->k[i] == k) {
        node_t * y = x->c[i];
        node_t * z = x->c[i+1];
        if(y->n >= x->t) {
            // Case 2a: y->n >= t
            // We don't allow duplicate keys, so we don't need to recurse.
            x->k[i] = y->k[y->n-1];
            x->v[i] = y->v[y->n-1];
            y->k[y->n-1] = INVALID_SENTINEL;
            y->v[y->n-1] = INVALID_SENTINEL;
            y->n--;
        } else if(z->n >= x->t) {
            // Case 2b: y->n < t, z->n >= t
            // We don't allow duplicate keys, so we don't need to recurse.
            x->k[i] = z->k[0];
            x->v[i] = z->v[0];
            for(j = 0; j < z->n-1; j++) {
                z->k[j] = z->k[j+1];
                z->v[j] = z->v[j+1];
            }
            z->k[z->n-1] = INVALID_SENTINEL;
            z->v[z->n-1] = INVALID_SENTINEL;
            z->n--;
        } else {
            // Case 2c: y->n < t, z->n < t
            y->k[y->n] = x->k[i];
            y->v[y->n] = x->v[i];
            y->n++;

            for(j = i; j < x->n-1; j++) {
                x->k[j] = x->k[j+1];
                x->v[j] = x->v[j+1];
                if(j > i) {
                    x->c[j] = x->c[j+1];
                }
            }
            x->k[j] = INVALID_SENTINEL;
            x->v[j] = INVALID_SENTINEL;
            x->c[j] = x->c[j+1];
            x->c[j+1] = NULL;
            x->n--;

            for(j = 0; j < z->n; j++) {
                y->k[y->n] = z->k[j];
                y->v[y->n] = z->v[j];
                y->c[y->n] = z->c[j];
                y->n++;
            }
            y->c[y->n] = z->c[j];

            destroy_node(z);
            btree_delete_from_subtree(y, k);
        }
        return;
    }

    // If this is a leaf, we have nowhere to go - the key isn't present.
    if(x->leaf) {
        return;
    }

    i++;
    node_t * child = x->c[i];
    if(child->n == x->t-1) {
        // Case 3: k not in x->k, find subtree, recurse
        if(i <= x->n && x->c[i+1]->n >= x->t) {
            // Case 3a: Immediate sibling (right) has at least t keys
            node_t * sib = x->c[i+1];

            // Give x->c[i] an extra key/value from x->k.
            child->k[child->n] = x->k[i];
            child->v[child->n] = x->v[i];
            child->n++;

            // Give x a key/value from right sibling.
            x->k[i] = sib->k[0];
            x->v[i] = sib->v[0];

            // Move child pointer from right sibling into child.
            child->c[child->n] = sib->c[0];
            
            // Shift right sibling keys/values/children down a slot.
            sib->n--;
            for(j = 0; j < sib->n; j++) {
                sib->k[j] = sib->k[j+1];
                sib->v[j] = sib->v[j+1];
                sib->c[j] = sib->c[j+1];
            }
            sib->k[sib->n] = INVALID_SENTINEL;
            sib->v[sib->n] = INVALID_SENTINEL;
            sib->c[sib->n] = sib->c[sib->n+1];
            sib->c[sib->n+1] = NULL;
        } else if (i > 0 && x->c[i-1]->n >= x->t) {
            // Case 3a: Immediate sibling (left) has at least t keys
            node_t * sib = x->c[i-1];

            // Give x->c[i] an extra key/value from x->k.
            child->k[child->n] = x->k[i];
            child->v[child->n] = x->v[i];
            child->n++;

            // Give x a key/value from left sibling.
            x->k[i] = x->k[i-1];
            x->v[i] = x->v[i-1];
            x->k[i-1] = sib->k[sib->n-1];
            x->v[i-1] = sib->v[sib->n-1];

            // Move child pointer from left sibling into child.
            child->c[0] = sib->c[sib->n];
            
            // Clear last key/value slot in left sibling.
            sib->n--;
            sib->k[sib->n] = INVALID_SENTINEL;
            sib->v[sib->n] = INVALID_SENTINEL;
            sib->c[sib->n+1] = NULL;
        } else if(i <= x->n) {
            // Case 3b: All immediate siblings have t-1 keys, merge with right
            node_t * sib = x->c[i+1];

            child->k[child->n] = x->k[i];
            child->v[child->n] = x->v[i];
            child->n++;

            for(j = 0; j < sib->n; j++, child->n++) {
                child->k[child->n] = sib->k[j];
                child->v[child->n] = sib->v[j];
                child->c[child->n] = sib->c[j];
            }
            child->c[child->n] = sib->c[j];

            for(j = x->n-1; j > i; j--) {
                x->k[j-1] = x->k[j];
                x->v[j-1] = x->v[j];
                x->c[j] = x->c[j+1];
            }
            x->n--;

            x->k[x->n] = INVALID_SENTINEL;
            x->v[x->n] = INVALID_SENTINEL;
            x->c[x->n+1] = NULL;

            free(sib);
        } else {
            // Case 3b: All immediate siblings have t-1 keys, merge with left
            node_t * sib = x->c[i-1];

            child->k[child->n] = x->k[i];
            child->v[child->n] = x->v[i];
            child->n++;

            for(j = 0; j < child->n; j++, sib->n++) {
                sib->k[sib->n] = child->k[j];
                sib->v[sib->n] = child->v[j];
                sib->c[sib->n] = child->c[j];
            }
            sib->c[sib->n] = child->c[j];

            for(j = x->n-1; j > i; j--) {
                x->k[j-1] = x->k[j];
                x->v[j-1] = x->v[j];
                x->c[j] = x->c[j+1];
            }
            x->n--;

            x->k[x->n] = INVALID_SENTINEL;
            x->v[x->n] = INVALID_SENTINEL;
            x->c[x->n+1] = NULL;

            free(sib);
        }
    }

    btree_delete_from_subtree(x->c[i], k);
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
