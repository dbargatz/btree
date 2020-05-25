#include "delete.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult delete_case_1(const MunitParameter inParams[], void * inFixture);
MunitResult delete_clrs_fig_18_8(const MunitParameter inParams[], void * inFixture);
MunitResult delete_invalid_ok(const MunitParameter inParams[], void * inFixture);
MunitResult delete_missing_ok(const MunitParameter inParams[], void * inFixture);
MunitResult delete_null_root_ok(const MunitParameter inParams[], void * inFixture);

MunitTest delete_tests[] = {
    { "/case_1", delete_case_1, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/clrs_fig_18_8", delete_clrs_fig_18_8, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/invalid_ok", delete_invalid_ok, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/missing_ok", delete_missing_ok, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
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

MunitResult delete_clrs_fig_18_8(const MunitParameter inParams[], void * inFixture) {
    // Figure 18.8a: Manually construct the tree shown.
    btree_t * T = btree_create(3);
    T->r->leaf = false; T->r->n = 1;
    T->r->k[0] = 'P'; T->r->v[0] = 'P';

    node_t * L0   = allocate_node(3);
    L0->leaf = false; L0->n = 3;
    L0->k[0] = 'C'; L0->v[0] = 'C';
    L0->k[1] = 'G'; L0->v[1] = 'G';
    L0->k[2] = 'M'; L0->v[2] = 'M';
    T->r->c[0] = L0;

    node_t * L0_0 = allocate_node(3);
    L0_0->leaf = true; L0_0->n = 2;
    L0_0->k[0] = 'A'; L0_0->v[0] = 'A';
    L0_0->k[1] = 'B'; L0_0->v[1] = 'B';
    L0->c[0] = L0_0;

    node_t * L0_1 = allocate_node(3);
    L0_1->leaf = true; L0_1->n = 3;
    L0_1->k[0] = 'D'; L0_1->v[0] = 'D';
    L0_1->k[1] = 'E'; L0_1->v[1] = 'E';
    L0_1->k[2] = 'F'; L0_1->v[2] = 'F';
    L0->c[1] = L0_1;

    node_t * L0_2 = allocate_node(3);
    L0_2->leaf = true; L0_2->n = 3;
    L0_2->k[0] = 'J'; L0_2->v[0] = 'J';
    L0_2->k[1] = 'K'; L0_2->v[1] = 'K';
    L0_2->k[2] = 'L'; L0_2->v[2] = 'L';
    L0->c[2] = L0_2;

    node_t * L0_3 = allocate_node(3);
    L0_3->leaf = true; L0_3->n = 2;
    L0_3->k[0] = 'N'; L0_3->v[0] = 'N';
    L0_3->k[1] = 'O'; L0_3->v[1] = 'O';
    L0->c[3] = L0_3;

    node_t * L1   = allocate_node(3);
    L1->leaf = false; L1->n = 2;
    L1->k[0] = 'T'; L1->v[0] = 'T';
    L1->k[1] = 'X'; L1->v[1] = 'X';
    T->r->c[1] = L1;

    node_t * L1_0 = allocate_node(3);
    L1_0->leaf = true; L1_0->n = 3;
    L1_0->k[0] = 'Q'; L1_0->v[0] = 'Q';
    L1_0->k[1] = 'R'; L1_0->v[1] = 'R';
    L1_0->k[2] = 'S'; L1_0->v[2] = 'S';
    L1->c[0] = L1_0;

    node_t * L1_1 = allocate_node(3);
    L1_1->leaf = true; L1_1->n = 2;
    L1_1->k[0] = 'U'; L1_1->v[0] = 'U';
    L1_1->k[1] = 'V'; L1_1->v[1] = 'V';
    L1->c[1] = L1_1;

    node_t * L1_2 = allocate_node(3);
    L1_2->leaf = true; L1_2->n = 2;
    L1_2->k[0] = 'Y'; L1_2->v[0] = 'Y';
    L1_2->k[1] = 'Z'; L1_2->v[1] = 'Z';
    L1->c[2] = L1_2;

    // Figure 18.8a: Ensure that the manually-constructed initial tree is valid.
    munit_log(MUNIT_LOG_INFO, "Running 18.8a");
    assert_tree_valid(T);

    // Figure 18.8b: Delete 'F', validate case 1.
    munit_log(MUNIT_LOG_INFO, "Running 18.8b");
    btree_delete(T->r, 'F');
    assert_tree_valid(T);
    assert_search_failed(btree_search(T->r, 'F'));
    munit_assert_uint16(L0_1->n, ==, 2);
    munit_assert_char(L0_1->k[0], ==, 'D'); munit_assert_char(L0_1->v[0], ==, 'D');
    munit_assert_char(L0_1->k[1], ==, 'E'); munit_assert_char(L0_1->v[1], ==, 'E');

    // Figure 18.8c: Delete 'M', validate case 2a.
    munit_log(MUNIT_LOG_INFO, "Running 18.8c");
    btree_delete(T->r, 'M');
    assert_tree_valid(T);
    assert_search_failed(btree_search(T->r, 'M'));
    munit_assert_uint16(L0->n, ==, 3);
    munit_assert_char(L0->k[0], ==, 'C'); munit_assert_char(L0->v[0], ==, 'C');
    munit_assert_char(L0->k[1], ==, 'G'); munit_assert_char(L0->v[1], ==, 'G');
    munit_assert_char(L0->k[2], ==, 'L'); munit_assert_char(L0->v[2], ==, 'L');
    munit_assert_uint16(L0->c[2]->n, ==, 2);
    munit_assert_char(L0->c[2]->k[0], ==, 'J'); munit_assert_char(L0->c[2]->v[0], ==, 'J');
    munit_assert_char(L0->c[2]->k[1], ==, 'K'); munit_assert_char(L0->c[2]->v[1], ==, 'K');

    // Figure 18.8d: Delete 'G', validate case 2c.
    munit_log(MUNIT_LOG_INFO, "Running 18.8d");
    btree_delete(T->r, 'G');
    assert_tree_valid(T);
    assert_search_failed(btree_search(T->r, 'G'));
    munit_assert_uint16(L0->n, ==, 2);
    munit_assert_char(L0->k[0], ==, 'C'); munit_assert_char(L0->v[0], ==, 'C');
    munit_assert_char(L0->k[1], ==, 'L'); munit_assert_char(L0->v[1], ==, 'L');
    munit_assert_uint16(L0->c[1]->n, ==, 4);
    munit_assert_char(L0->c[1]->k[0], ==, 'D'); munit_assert_char(L0->c[1]->v[0], ==, 'D');
    munit_assert_char(L0->c[1]->k[1], ==, 'E'); munit_assert_char(L0->c[1]->v[1], ==, 'E');
    munit_assert_char(L0->c[1]->k[2], ==, 'J'); munit_assert_char(L0->c[1]->v[2], ==, 'J');
    munit_assert_char(L0->c[1]->k[3], ==, 'K'); munit_assert_char(L0->c[1]->v[3], ==, 'K');

    // Figure 18.8e/e': Delete 'D', validate case 3b.
    munit_log(MUNIT_LOG_INFO, "Running 18.8e/e'");
    btree_delete(T->r, 'D');
    assert_tree_valid(T);
    assert_search_failed(btree_search(T->r, 'D'));
    munit_assert_uint16(T->r->n, ==, 5);
    munit_assert_char(T->r->k[0], ==, 'C'); munit_assert_char(T->r->v[0], ==, 'C');
    munit_assert_char(T->r->k[1], ==, 'L'); munit_assert_char(T->r->v[1], ==, 'L');
    munit_assert_char(T->r->k[2], ==, 'P'); munit_assert_char(T->r->v[2], ==, 'P');
    munit_assert_char(T->r->k[3], ==, 'T'); munit_assert_char(T->r->v[3], ==, 'T');
    munit_assert_char(T->r->k[4], ==, 'X'); munit_assert_char(T->r->v[4], ==, 'X');
    munit_assert_uint16(T->r->c[0]->n, ==, 2);
    munit_assert_char(T->r->c[0]->k[0], ==, 'A'); munit_assert_char(T->r->c[0]->v[0], ==, 'A');
    munit_assert_char(T->r->c[0]->k[1], ==, 'B'); munit_assert_char(T->r->c[0]->v[1], ==, 'B');
    munit_assert_uint16(T->r->c[1]->n, ==, 3);
    munit_assert_char(T->r->c[1]->k[0], ==, 'E'); munit_assert_char(T->r->c[1]->v[0], ==, 'E');
    munit_assert_char(T->r->c[1]->k[1], ==, 'J'); munit_assert_char(T->r->c[1]->v[1], ==, 'J');
    munit_assert_char(T->r->c[1]->k[2], ==, 'K'); munit_assert_char(T->r->c[1]->v[2], ==, 'K');
    munit_assert_uint16(T->r->c[2]->n, ==, 2);
    munit_assert_char(T->r->c[2]->k[0], ==, 'N'); munit_assert_char(T->r->c[2]->v[0], ==, 'N');
    munit_assert_char(T->r->c[2]->k[1], ==, 'O'); munit_assert_char(T->r->c[2]->v[1], ==, 'O');
    munit_assert_uint16(T->r->c[3]->n, ==, 3);
    munit_assert_char(T->r->c[3]->k[0], ==, 'Q'); munit_assert_char(T->r->c[3]->v[0], ==, 'Q');
    munit_assert_char(T->r->c[3]->k[1], ==, 'R'); munit_assert_char(T->r->c[3]->v[1], ==, 'R');
    munit_assert_char(T->r->c[3]->k[2], ==, 'S'); munit_assert_char(T->r->c[3]->v[2], ==, 'S');
    munit_assert_uint16(T->r->c[4]->n, ==, 2);
    munit_assert_char(T->r->c[4]->k[0], ==, 'U'); munit_assert_char(T->r->c[4]->v[0], ==, 'U');
    munit_assert_char(T->r->c[4]->k[1], ==, 'V'); munit_assert_char(T->r->c[4]->v[1], ==, 'V');
    munit_assert_uint16(T->r->c[5]->n, ==, 2);
    munit_assert_char(T->r->c[5]->k[0], ==, 'Y'); munit_assert_char(T->r->c[5]->v[0], ==, 'Y');
    munit_assert_char(T->r->c[5]->k[1], ==, 'Z'); munit_assert_char(T->r->c[5]->v[1], ==, 'Z');

    // Figure 18.8f: Delete 'B', validate case 3a.
    munit_log(MUNIT_LOG_INFO, "Running 18.8f");
    btree_delete(T->r, 'B');
    assert_tree_valid(T);
    assert_search_failed(btree_search(T->r, 'B'));
    munit_assert_uint16(T->r->n, ==, 5);
    munit_assert_char(T->r->k[0], ==, 'E'); munit_assert_char(T->r->v[0], ==, 'E');
    munit_assert_char(T->r->k[1], ==, 'L'); munit_assert_char(T->r->v[1], ==, 'L');
    munit_assert_char(T->r->k[2], ==, 'P'); munit_assert_char(T->r->v[2], ==, 'P');
    munit_assert_char(T->r->k[3], ==, 'T'); munit_assert_char(T->r->v[3], ==, 'T');
    munit_assert_char(T->r->k[4], ==, 'X'); munit_assert_char(T->r->v[4], ==, 'X');
    munit_assert_uint16(T->r->c[0]->n, ==, 2);
    munit_assert_char(T->r->c[0]->k[0], ==, 'A'); munit_assert_char(T->r->c[0]->v[0], ==, 'A');
    munit_assert_char(T->r->c[0]->k[1], ==, 'C'); munit_assert_char(T->r->c[0]->v[1], ==, 'C');
    munit_assert_uint16(T->r->c[1]->n, ==, 3);
    munit_assert_char(T->r->c[1]->k[0], ==, 'J'); munit_assert_char(T->r->c[1]->v[0], ==, 'J');
    munit_assert_char(T->r->c[1]->k[1], ==, 'K'); munit_assert_char(T->r->c[1]->v[1], ==, 'K');
    munit_assert_uint16(T->r->c[2]->n, ==, 2);
    munit_assert_char(T->r->c[2]->k[0], ==, 'N'); munit_assert_char(T->r->c[2]->v[0], ==, 'N');
    munit_assert_char(T->r->c[2]->k[1], ==, 'O'); munit_assert_char(T->r->c[2]->v[1], ==, 'O');
    munit_assert_uint16(T->r->c[3]->n, ==, 3);
    munit_assert_char(T->r->c[3]->k[0], ==, 'Q'); munit_assert_char(T->r->c[3]->v[0], ==, 'Q');
    munit_assert_char(T->r->c[3]->k[1], ==, 'R'); munit_assert_char(T->r->c[3]->v[1], ==, 'R');
    munit_assert_char(T->r->c[3]->k[2], ==, 'S'); munit_assert_char(T->r->c[3]->v[2], ==, 'S');
    munit_assert_uint16(T->r->c[4]->n, ==, 2);
    munit_assert_char(T->r->c[4]->k[0], ==, 'U'); munit_assert_char(T->r->c[4]->v[0], ==, 'U');
    munit_assert_char(T->r->c[4]->k[1], ==, 'V'); munit_assert_char(T->r->c[4]->v[1], ==, 'V');
    munit_assert_uint16(T->r->c[5]->n, ==, 2);
    munit_assert_char(T->r->c[5]->k[0], ==, 'Y'); munit_assert_char(T->r->c[5]->v[0], ==, 'Y');
    munit_assert_char(T->r->c[5]->k[1], ==, 'Z'); munit_assert_char(T->r->c[5]->v[1], ==, 'Z');

    // Destroy the manually-created tree to avoid leaks.
    btree_destroy(T);

    return MUNIT_OK;
}

MunitResult delete_invalid_ok(const MunitParameter inParams[], void * inFixture) {
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

MunitResult delete_missing_ok(const MunitParameter inParams[], void * inFixture) {
    // Ensure the root is a leaf, is full, and is valid.
    btree_t * T = (btree_t *)inFixture;
    node_t * r = T->r;
    munit_assert(r->leaf);
    munit_assert(r->n == (2*T->t)-1);

    // Generate a key guaranteed not to be in the tree.
    uint64_t k = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);

    // Attempt to delete the missing key.
    btree_delete(T->r, k);

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
