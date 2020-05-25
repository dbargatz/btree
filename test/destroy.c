#include "destroy.h"
#include "../src/btree.h"

MunitResult null_tree_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult valid_tree_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult null_root_succeeds(const MunitParameter inParams[], void * inFixture);

MunitTest destroy_tests[] = {
    { "/null_tree_succeeds", null_tree_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/valid_tree_succeeds", valid_tree_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_succeeds", null_root_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult null_tree_succeeds(const MunitParameter inParams[], void * inFixture) {
    // This will segfault if NULL isn't specially handled. If we get past the 
    // call, it was successful.
    btree_destroy(NULL);
    return MUNIT_OK;
}

MunitResult valid_tree_succeeds(const MunitParameter inParams[], void * inFixture) {
    // Randomized min degree ensures btree_destroy() doesn't succeed in some cases
    // and fail in others.
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, MIN_DEGREE_MAX);
    btree_t * T = btree_create(t);
    if(NULL == T) {
        // Return error because create failed, but we're testing destroy.
        return MUNIT_ERROR;
    }
    // There's nothing we can do if this fails, so assume if it doesn't crash
    // we're good.
    btree_destroy(T);
    return MUNIT_OK;
}

MunitResult null_root_succeeds(const MunitParameter inParams[], void * inFixture) {
    // Rather than use btree_create(), just allocate a btree_t struct and 
    // populate it with wacky data/NULL for root.
    btree_t * T = munit_malloc(sizeof(btree_t));
    if(NULL == T) {
        // Return error because malloc failed, but we're testing destroy.
        return MUNIT_ERROR;
    }
    T->t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, MIN_DEGREE_MAX);
    T->r = NULL;
    // There's nothing we can do if this fails, so assume if it doesn't crash
    // we're good.
    btree_destroy(T);
    return MUNIT_OK;
}
