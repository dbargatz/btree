#include "search.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult found_succeeds (const MunitParameter inParams[], void * inFixture);
MunitResult not_found_fails (const MunitParameter inParams[], void * inFixture);
MunitResult null_root_fails (const MunitParameter inParams[], void * inFixture);

MunitTest search_tests[] = {
    { "/found_succeeds", found_succeeds, setup_medium, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/not_found_fails", not_found_fails, setup_medium, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_fails", null_root_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult found_succeeds (const MunitParameter inParams[], void * inFixture) {
    // Generate a test key guaranteed to NOT be in the tree and insert it, using
    // the key itself as the value.
    btree_t * T = (btree_t *)inFixture;
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_insert(T, k, k);

    // Search for the key and make sure the result is valid, and the result points
    // to the correct value.
    btree_search_result_t result = btree_search(T->r, k);
    munit_assert_ptr_not_null(result.x);
    munit_assert_uint64(result.x->k[result.i], ==, k);
    return MUNIT_OK;
}

MunitResult not_found_fails (const MunitParameter inParams[], void * inFixture) {
    // Generate a test key guaranteed to NOT be in the tree.
    btree_t * T = (btree_t *)inFixture;
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);

    // Search for the key and make sure it isn't found.
    assert_search_failed(btree_search(T->r, k));
    return MUNIT_OK;
}

MunitResult null_root_fails (const MunitParameter inParams[], void * inFixture) {
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    assert_search_failed(btree_search(NULL, k));
    return MUNIT_OK;
}
