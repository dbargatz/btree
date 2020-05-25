#include "delete.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult delete_null_root_ok(const MunitParameter inParams[], void * inFixture);
MunitResult delete_null_tree_ok(const MunitParameter inParams[], void * inFixture);

MunitTest delete_tests[] = {
    { "/null_root_ok", delete_null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_tree_ok", delete_null_tree_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult delete_null_root_ok(const MunitParameter inParams[], void * inFixture) {
    // So long as this doesn't crash, we'll consider it OK.
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_delete(NULL, k);
    return MUNIT_OK;
}

MunitResult delete_null_tree_ok(const MunitParameter inParams[], void * inFixture) {
    btree_t * T = (btree_t *)munit_malloc(sizeof(*T));
    T->t = MIN_DEGREE_MIN;
    T->r = NULL;

    // So long as this doesn't crash, we'll consider it OK.
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_delete(T, k);

    // Free the tree to avoid leaks.
    free(T);
    return MUNIT_OK;
}
