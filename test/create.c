#include "create.h"
#include "../src/btree.h"

MunitResult degree_lt_2_fails(const MunitParameter inParams[], void * inFixture);
MunitResult degree_gte_2_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult root_exists(const MunitParameter inParams[], void * inFixture);
MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture);
MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture);

MunitTest create_tests[] = {
    { "/degree_lt_2_fails", degree_lt_2_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/degree_gte_2_succeeds", degree_gte_2_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_exists", root_exists, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_leaf", root_is_leaf, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_empty", root_is_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult degree_lt_2_fails(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(0, 1);
    if(NULL != btree_create(t)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult degree_gte_2_succeeds(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    if(NULL == btree_create(t)) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_exists(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    btree_t * tree = btree_create(t);

    if(NULL == tree->r) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    btree_t * tree = btree_create(t);

    if(NULL != tree->r->c[0]) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    btree_t * tree = btree_create(t);

    if(0 != tree->r->n) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}