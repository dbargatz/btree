#include "create.h"
#include "../src/btree.h"

MunitTest create_tests[] = {
    { "/order_lt_2_fails", order_lt_2_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/order_gte_2_succeeds", order_gte_2_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_exists", root_exists, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_leaf", root_is_leaf, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_empty", root_is_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult order_lt_2_fails(const MunitParameter inParams[], void * inFixture) {
    uint16_t order = (uint16_t)munit_rand_int_range(0, 1);
    if(NULL != btree_create(order)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult order_gte_2_succeeds(const MunitParameter inParams[], void * inFixture) {
    uint16_t order = (uint16_t)munit_rand_int_range(2, 65535);
    if(NULL == btree_create(order)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_exists(const MunitParameter inParams[], void * inFixture) {
    uint16_t order = (uint16_t)munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);

    if(NULL == tree->root) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture) {
    uint16_t order = (uint16_t)munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);

    if(NULL != tree->root->children[0]) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture) {
    uint16_t order = (uint16_t)munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);

    if(0 != tree->root->numKeys) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}