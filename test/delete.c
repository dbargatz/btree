#include "delete.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult delete_invalid_fails(const MunitParameter inParams[], void * inFixture);
MunitResult delete_null_root_ok(const MunitParameter inParams[], void * inFixture);

MunitTest delete_tests[] = {
    { "/invalid_fails", delete_invalid_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_ok", delete_null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult delete_invalid_fails(const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is a leaf, is full, and is valid.
    btree_t * T = (btree_t *)inFixture;
    node_t * r = T->r;
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);

    // Attempt to delete an INVALID_SENTINEL key.
    btree_delete(T->r, INVALID_SENTINEL);

    // Validate the root is the same node, is still a leaf, is full, and is 
    // valid. This means nothing was deleted or re-balanced.
    munit_assert_ptr_equal(T->r, r);
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);
    assert_tree_valid(T);
    return MUNIT_OK;
}

MunitResult delete_null_root_ok(const MunitParameter inParams[], void * inFixture) {
    // So long as this doesn't crash, we'll consider it OK.
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_delete(NULL, k);
    return MUNIT_OK;
}
