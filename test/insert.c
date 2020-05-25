#include "insert.h"
#include "utility.h"
#include "../src/btree.h"

// TODO: add example 18.2-1 (p. 497)
// TODO: add figure 18.7, verify
// TODO: add duplicate key overwrites

MunitResult full_causes_split (const MunitParameter inParams[], void * inFixture);
MunitResult insert_invalid_key_fails (const MunitParameter inParams[], void * inFixture);
MunitResult insert_invalid_value_fails (const MunitParameter inParams[], void * inFixture);
MunitResult non_full_doesnt_split (const MunitParameter inParams[], void * inFixture);
MunitResult null_tree_ok (const MunitParameter inParams[], void * inFixture);
MunitResult null_root_ok (const MunitParameter inParams[], void * inFixture);

MunitTest insert_tests[] = {
    { "/full_causes_split", full_causes_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/insert_invalid_key_fails", insert_invalid_key_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/insert_invalid_value_fails", insert_invalid_value_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/non_full_doesnt_split", non_full_doesnt_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_tree_ok", null_tree_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_ok", null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult full_causes_split(const MunitParameter inParams[], void * inFixture) {
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);

    // Ensure the root is a leaf, is full, and is valid.
    btree_t * T = (btree_t *)inFixture;
    munit_assert(T->r->leaf);
    munit_assert(T->r->n == (2*T->t)-1);
    assert_tree_valid(T->r);

    // Insert one more key to cause a split.
    btree_insert(T, k, k);

    // Validate the root is no longer a leaf, and the whole tree is valid.
    munit_assert(!T->r->leaf);
    assert_tree_valid(T->r);

    return MUNIT_OK;
}

MunitResult insert_invalid_key_fails (const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is a leaf, is full, and is valid.
    btree_t * T = (btree_t *)inFixture;
    node_t * r = T->r;
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);

    // Attempt to insert an INVALID_SENTINEL key with a valid value.
    uint64_t v = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_insert(T, INVALID_SENTINEL, v);

    // Validate the root is the same node, is still a leaf, is full, and is 
    // valid. This means the key wasn't inserted.
    munit_assert_ptr_equal(T->r, r);
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);
    assert_tree_valid(T->r);
    return MUNIT_OK;
}

MunitResult insert_invalid_value_fails (const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is a leaf, is full, and is valid.
    btree_t * T = (btree_t *)inFixture;
    node_t * r = T->r;
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);

    // Attempt to insert a valid key with an INVALID_SENTINEL value.
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_insert(T, k, INVALID_SENTINEL);

    // Validate the root is the same node, is still a leaf, is full, and is 
    // valid. This means the key with the invalid value wasn't inserted.
    munit_assert_ptr_equal(T->r, r);
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);
    assert_tree_valid(T->r);
    return MUNIT_OK;
}

MunitResult null_tree_ok(const MunitParameter inParams[], void * inFixture) {
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_insert(NULL, k, k);
    return MUNIT_OK;
}

MunitResult null_root_ok(const MunitParameter inParams[], void * inFixture) {
    btree_t * T = (btree_t *)munit_malloc(sizeof(*T));
    T->t = MIN_DEGREE_MIN;
    T->r = NULL;

    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_insert(T, k, k);
    return MUNIT_OK;
}

MunitResult non_full_doesnt_split(const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is valid and a leaf.
    btree_t * T = (btree_t *)inFixture;
    munit_assert(T->r->leaf);
    munit_assert(T->r->n == (2*T->t)-1);
    assert_tree_valid(T->r);

    return MUNIT_OK;
}
