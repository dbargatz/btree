#include "insert.h"
#include "utility.h"
#include "../src/btree.h"

// TODO: add example 18.2-1 (p. 497)
// TODO: add figure 18.7, verify

MunitResult duplicate_overwrites (const MunitParameter inParams[], void * inFixture);
MunitResult full_causes_split (const MunitParameter inParams[], void * inFixture);
MunitResult invalid_key_fails (const MunitParameter inParams[], void * inFixture);
MunitResult invalid_value_fails (const MunitParameter inParams[], void * inFixture);
MunitResult non_full_doesnt_split (const MunitParameter inParams[], void * inFixture);
MunitResult null_tree_ok (const MunitParameter inParams[], void * inFixture);
MunitResult null_root_ok (const MunitParameter inParams[], void * inFixture);

MunitTest insert_tests[] = {
    { "/duplicate_overwrites", duplicate_overwrites, setup_medium, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/full_causes_split", full_causes_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/invalid_key_fails", invalid_key_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/invalid_value_fails", invalid_value_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/non_full_doesnt_split", non_full_doesnt_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_tree_ok", null_tree_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_ok", null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult duplicate_overwrites(const MunitParameter inParams[], void * inFixture) {
    btree_search_result_t result;
    btree_t * T = (btree_t *)inFixture;

    // Generate a key and two different values guaranteed not to be in the tree.
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    uint64_t v1 = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
    uint64_t v2 = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);

    // Unlikely, but if v1==v2, error out of the test.
    if(v1 == v2) {
        munit_errorf("v1 == v2 (0x%016" PRIX64 ")", v1);
        return MUNIT_ERROR;
    }

    // Insert the key with the first value.
    btree_insert(T, k, v1);

    // Validate that the key was inserted and has the correct value.
    result = btree_search(T->r, k);
    munit_assert_ptr_not_null(result.x);
    munit_assert_uint64(v1, ==, result.x->v[result.i]);

    // Re-insert the same key, but using a different value.
    btree_insert(T, k, v2);

    // Validate that the new value overwrote the old value.
    result = btree_search(T->r, k);
    munit_assert_ptr_not_null(result.x);
    munit_assert_uint64(v1, !=, result.x->v[result.i]);
    munit_assert_uint64(v2, ==, result.x->v[result.i]);

    return MUNIT_OK;
}

MunitResult full_causes_split(const MunitParameter inParams[], void * inFixture) {
    // Generate a key guaranteed not to be in the tree.
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

MunitResult invalid_key_fails (const MunitParameter inParams[], void * inFixture) {
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

MunitResult invalid_value_fails (const MunitParameter inParams[], void * inFixture) {
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
