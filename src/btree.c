#include "btree.h"
#include <stdlib.h>   // for malloc, free
#include <string.h>   // for memset

node_t * allocate_node(uint16_t t);
void destroy_node(node_t * x);
void btree_insert_nonfull(node_t * x, uint64_t k, uint64_t v);
void btree_split_child(node_t * x, uint16_t i);

node_t * allocate_node(uint16_t t) {
    node_t * x;
    uint64_t kBytes = sizeof(*x->k) * ((2*t)-1);
    uint64_t vBytes = sizeof(*x->v) * ((2*t)-1);
    uint64_t cBytes = sizeof(*x->c) * (2*t);
    x = malloc(sizeof(*x) + kBytes + vBytes + cBytes);

    x->n = 0;
    x->t = t;
    x->k = (uint64_t *)(((uint8_t *)x) + sizeof(*x));
    x->v = (uint64_t *)(((uint8_t *)x->k) + kBytes);
    x->c = (node_t **)(((uint8_t *)x->v) + vBytes);
    memset(x->c, 0, cBytes);
    return x;
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
    return;
}

void btree_destroy(btree_t * inTree) {
    if(NULL == inTree) {
        return;
    }
    destroy_node(inTree->r);
    free(inTree);
}

void btree_insert(btree_t * T, uint64_t k, uint64_t v) {
    node_t * s;

    if(NULL == T || NULL == T->r) {
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
            x->k[i+1] = x->k[i];
            i--;
        }
        x->k[i+1] = k;
        x->v[i+1] = v;
        x->n++;
    } else {
        while(i >= 0 && k < x->k[i]) {
            i--;
        }
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
    }

    if(!y->leaf) {
        for(j = 0; j < x->t; j++) {
            z->c[j] = y->c[j+x->t];
        }
    }
    y->n = x->t-1;

    for(j = x->n; j > i; j--) {
        x->c[j+1] = x->c[j];
    }
    x->c[i+1] = z;

    for(j = x->n - 1; j >= i; j--) {
        x->k[j+1] = x->k[j];
    }
    x->k[i] = y->k[x->t-1];
    x->n++;
}
