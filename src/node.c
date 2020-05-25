#include "node.h"
#include <stdlib.h>
#include <string.h>

node_t * node_create(uint16_t t) {
    node_t * x;
    uint64_t keyLength = sizeof(*x->k) * ((2 * t)-1);
    uint64_t valLength = sizeof(*x->values) * ((2 * t)-1);
    uint64_t childrenLength = sizeof(*x->c) * (2 * t);
    x = malloc(sizeof(node_t) + keyLength + valLength + childrenLength);

    x->n = 0;
    x->t = t;
    x->k = (uint64_t *)(((uint8_t *)x) + sizeof(node_t));
    x->values = (uint64_t *)(((uint8_t *)x->k) + keyLength);
    x->c = (node_t **)(((uint8_t *)x->values) + valLength);
    memset(x->c, 0, childrenLength);
    return x;
}

void node_destroy(node_t * x) {
    int i;

    if(NULL == x) {
        return;
    }

    if(!x->leaf) {
        for(i = 0; i <= x->n; i++) {
            node_destroy(x->c[i]);
            x->c[i] = NULL;
        }
    }
    free(x);
}

void node_insert(node_t * x, uint64_t k, uint64_t inValue) {
    int i = x->n - 1;

    if(x->leaf) {
        while(i >= 0 && k < x->k[i]) {
            x->k[i+1] = x->k[i];
            i--;
        }
        x->k[i+1] = k;
        x->values[i+1] = inValue;
        x->n++;
    } else {
        while(i >= 0 && k < x->k[i]) {
            i--;
        }
        i++;
        if(x->c[i]->n == (2 * x->t) - 1) {
            node_split_child(x, i);
            if(k > x->k[i]) {
                i++;
            }
        }
        node_insert(x->c[i], k, inValue);
    }
}

search_result_t node_search(node_t * x, uint64_t k) {
    int i = 0;
    search_result_t result;

    if(NULL == x || INVALID_KEY_VALUE_SENTINEL == k) {
        result.node = NULL;
        result.valueIndex = 0;
        return result;
    }

    while(i < x->n && k > x->k[i]) {
        i++;
    }
    if(i < x->n && k == x->k[i]) {
        result.node = x;
        result.valueIndex = i;
        return result;
    } else if(x->leaf) {
        result.node = NULL;
        result.valueIndex = 0;
        return result;
    } else {
        return node_search(x->c[i], k);
    }
}

void node_split_child(node_t * x, uint16_t i) {
    int j;
    int t = x->t;
    node_t * z = node_create(t);
    node_t * y = x->c[i];
    z->leaf = y->leaf;
    z->n = t - 1;

    for(j = 0; j < x->t-1; j++) {
        z->k[j] = y->k[j+x->t];
    }

    if(!y->leaf) {
        for(j = 0; j < t; j++) {
            z->c[j] = y->c[j+t];
        }
    }

    y->n = t-1;

    for(j = x->n; j > i; j--) {
        x->c[j+1] = x->c[j];
    }
    x->c[i+1] = z;
    for(j = x->n - 1; j >= i; j--) {
        x->k[j+1] = x->k[j];
    }
    x->k[i] = y->k[t-1];
    x->n++;
}