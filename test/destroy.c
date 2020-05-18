#include "destroy.h"
#include "../src/btree.h"

MunitTest destroy_tests[] = {
    { "/destroy_null_succeeds", destroy_null_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/destroy_valid_succeeds", destroy_valid_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult destroy_null_succeeds(const MunitParameter inParams[], void * inFixture) {
    // This will segfault if NULL isn't specially handled. If we get past the 
    // call, it was successful.
    btree_destroy(NULL);
    return MUNIT_OK;
}

MunitResult destroy_valid_succeeds(const MunitParameter inParams[], void * inFixture) {
    // Randomized order ensures btree_destroy() doesn't succeed in some cases
    // and fail in others.
    uint16_t order = (uint16_t)munit_rand_int_range(2, 65535);
    btree_t * tree = btree_create(order);
    if(NULL == tree) {
        // Return error because create failed, but we're testing destroy.
        return MUNIT_ERROR;
    }
    // There's nothing we can do if this fails, so assume if it doesn't crash
    // we're good.
    btree_destroy(tree);
    return MUNIT_OK;
}
