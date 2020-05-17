#include "create.h"
#include "../src/btree.h"

MunitResult order_lt_2_fails(const MunitParameter inParams[], void * inFixture) {
    int order = munit_rand_int_range(-65535, 1);
    if(NULL != btree_create(order)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult order_gte_2_succeeds(const MunitParameter inParams[], void * inFixture) {
    int order = munit_rand_int_range(2, 65535);
    if(NULL == btree_create(order)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult order_gt_65535_fails(const MunitParameter inParams[], void * inFixture) {
    int order = munit_rand_int_range(65536, INT32_MAX-1);
    if(NULL != btree_create(order)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_exists(const MunitParameter inParams[], void * inFixture) {
    int order = munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);

    if(NULL == tree->root) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture) {
    int order = munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);

    if(NULL != tree->root->children[0]) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture) {
    int order = munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);

    if(0 != tree->root->numKeys) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}