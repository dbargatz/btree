#include "delete.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult delete_case_1(const MunitParameter inParams[], void * inFixture);
MunitResult delete_invalid_fails(const MunitParameter inParams[], void * inFixture);
MunitResult delete_null_root_ok(const MunitParameter inParams[], void * inFixture);

MunitTest delete_tests[] = {
    { "/case_1", delete_case_1, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/invalid_fails", delete_invalid_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_ok", delete_null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult delete_case_1(const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is a leaf, is full, and is valid.
    btree_t * T = (btree_t *)inFixture;
    node_t * r = T->r;
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);

    // Select a random key from the root node to delete.
    uint16_t ki = (uint16_t)munit_rand_int_range(0, r->n-1);
    uint64_t k = r->k[ki];

    // Ensure that a search returns a valid and expected result.
    btree_search_result_t before = btree_search(T->r, k);
    munit_assert_ptr_not_null(before.x);
    munit_assert_uint64(before.x->k[before.i], ==, k);

    // Delete the random key.
    btree_delete(T->r, k);

    // Ensure that the search now returns failure.
    assert_search_failed(btree_search(T->r, k));

    // Validate the root is the same node, is still a leaf, is valid, but is no
    // longer full, and has one empty key slot. This means the selected key was
    // deleted.
    munit_assert_ptr_equal(T->r, r);
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-2);
    assert_tree_valid(T);
    return MUNIT_OK;
}

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
