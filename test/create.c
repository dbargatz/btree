#include "create.h"
#include "../src/btree.h"

MunitResult min_degree_too_small_fails(const MunitParameter inParams[], void * inFixture);
MunitResult min_degree_valid_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult min_degree_too_big_fails(const MunitParameter inParams[], void * inFixture);
MunitResult root_exists(const MunitParameter inParams[], void * inFixture);
MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture);
MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture);

MunitTest create_tests[] = {
    { "/min_degree_too_small_fails", min_degree_too_small_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/min_degree_valid_succeeds", min_degree_valid_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/min_degree_too_big_fails", min_degree_too_big_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_exists", root_exists, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_leaf", root_is_leaf, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_empty", root_is_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult min_degree_too_small_fails(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(0, MIN_DEGREE_MIN-1);
    munit_assert_ptr_null(btree_create(t));
    return MUNIT_OK;
}

MunitResult min_degree_valid_succeeds(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, MIN_DEGREE_MAX);
    btree_t * T = btree_create(t);
    munit_assert_ptr_not_null(T);
    btree_destroy(T);
    return MUNIT_OK;
}

MunitResult min_degree_too_big_fails(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MAX+1, UINT16_MAX);
    munit_assert_ptr_null(btree_create(t));
    return MUNIT_OK;
}

MunitResult root_exists(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, MIN_DEGREE_MAX);
    btree_t * T = btree_create(t);
    munit_assert_ptr_not_null(T->r);
    btree_destroy(T);
    return MUNIT_OK;
}

MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, MIN_DEGREE_MAX);
    btree_t * T = btree_create(t);
    munit_assert(T->r->leaf);
    btree_destroy(T);
    return MUNIT_OK;
}

MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, MIN_DEGREE_MAX);
    btree_t * T = btree_create(t);
    munit_assert_uint16(T->r->n, ==, 0);
    btree_destroy(T);
    return MUNIT_OK;
}
