#include "insert.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult null_tree_ok (const MunitParameter inParams[], void * inFixture);
MunitResult null_root_ok (const MunitParameter inParams[], void * inFixture);
MunitResult non_full_doesnt_split (const MunitParameter inParams[], void * inFixture);
MunitResult full_causes_split (const MunitParameter inParams[], void * inFixture);

MunitTest insert_tests[] = {
    { "/null_tree_ok", null_tree_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_ok", null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/non_full_doesnt_split", non_full_doesnt_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/full_causes_split", full_causes_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult null_tree_ok(const MunitParameter inParams[], void * inFixture) {
    btree_insert(NULL, (uint64_t)munit_rand_uint32(), 0);
    return MUNIT_OK;
}

MunitResult null_root_ok(const MunitParameter inParams[], void * inFixture) {
    btree_t * tree = (btree_t *)munit_malloc(sizeof(*tree));
    tree->t = DEGREE_MIN;
    tree->r = NULL;
    btree_insert(tree, (uint64_t)munit_rand_uint32(), 0);
    return MUNIT_OK;
}

MunitResult non_full_doesnt_split(const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is valid and a leaf.
    btree_t * tree = (btree_t *)inFixture;
    munit_assert(tree->r->leaf);
    munit_assert(tree->r->n == (2 * tree->r->t) - 1);
    assert_tree_valid(tree->r);

    return MUNIT_OK;
}

MunitResult full_causes_split(const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is a leaf and is valid.
    btree_t * tree = (btree_t *)inFixture;
    munit_assert(tree->r->leaf);
    munit_assert(tree->r->n == (2 * tree->r->t) - 1);
    assert_tree_valid(tree->r);

    // Insert one more key to cause a split.
    btree_insert(tree, (uint64_t)munit_rand_uint32(), 0);

    // Validate the root is no longer a leaf, and the whole tree is valid.
    munit_assert(!tree->r->leaf);
    assert_tree_valid(tree->r);

    return MUNIT_OK;
}
