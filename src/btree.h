#ifndef _BTREE_H
#define _BTREE_H

#include "node.h"

typedef struct _btree {
    uint16_t t;
    node_t * r;
} btree_t;

btree_t * btree_create(uint16_t t);
void btree_destroy(btree_t * inTree);
void btree_insert(btree_t * inTree, uint64_t inKey, uint64_t inValue);
uint64_t btree_search(const btree_t * inTree, uint64_t inKey);

#endif // _BTREE_H