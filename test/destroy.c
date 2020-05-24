#include "destroy.h"
#include "../src/btree.h"

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
    uint16_t t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    btree_t * tree = btree_create(t);
    if(NULL == tree) {
        // Return error because create failed, but we're testing destroy.
        return MUNIT_ERROR;
    }
    // There's nothing we can do if this fails, so assume if it doesn't crash
    // we're good.
    btree_destroy(tree);
    return MUNIT_OK;
}

MunitResult null_root_succeeds(const MunitParameter inParams[], void * inFixture) {
    // Rather than use btree_create(), just allocate a btree_t struct and 
    // populate it with wacky data/NULL for root.
    btree_t * tree = munit_malloc(sizeof(btree_t));
    if(NULL == tree) {
        // Return error because malloc failed, but we're testing destroy.
        return MUNIT_ERROR;
    }
    tree->t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    tree->r = NULL;
    // There's nothing we can do if this fails, so assume if it doesn't crash
    // we're good.
    btree_destroy(tree);
    return MUNIT_OK;
}
