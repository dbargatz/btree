#include "insert.h"
#include "utility.h"
#include "../src/btree.h"

MunitResult clrs_ex_18_2_1(const MunitParameter inParams[], void * inFixture);
MunitResult clrs_fig_18_7(const MunitParameter inParams[], void * inFixture);
MunitResult duplicate_overwrites (const MunitParameter inParams[], void * inFixture);
MunitResult full_causes_split (const MunitParameter inParams[], void * inFixture);
MunitResult invalid_key_fails (const MunitParameter inParams[], void * inFixture);
MunitResult invalid_value_fails (const MunitParameter inParams[], void * inFixture);
MunitResult non_full_doesnt_split (const MunitParameter inParams[], void * inFixture);
MunitResult null_tree_ok (const MunitParameter inParams[], void * inFixture);
MunitResult null_root_ok (const MunitParameter inParams[], void * inFixture);

MunitTest insert_tests[] = {
    { "/clrs_ex_18_2_1", clrs_ex_18_2_1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/clrs_fig_18_7", clrs_fig_18_7, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/duplicate_overwrites", duplicate_overwrites, setup_medium, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/full_causes_split", full_causes_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/invalid_key_fails", invalid_key_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/invalid_value_fails", invalid_value_fails, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/non_full_doesnt_split", non_full_doesnt_split, setup_root, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_tree_ok", null_tree_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_ok", null_root_ok, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult clrs_ex_18_2_1(const MunitParameter inParams[], void * inFixture) {
    int i;
    btree_t * T = btree_create(2);
    char keys[] = {
        'F', 'S', 'Q', 'K', 'C', 'L', 'H', 'T', 'V', 'W', 'M',
        'R', 'N', 'P', 'A', 'B', 'X', 'Y', 'D', 'Z', 'E' 
    };

    for(i = 0; i < sizeof(keys); i++) {
        btree_insert(T, keys[i], keys[i]);
        assert_tree_valid(T);
    }

    return MUNIT_OK;
}

MunitResult clrs_fig_18_7(const MunitParameter inParams[], void * inFixture) {
    // Figure 18.7a: Manually construct the tree shown.
    btree_t * T = btree_create(3);
    T->r->leaf = false; T->r->n = 4;
    T->r->k[0] = 'G'; T->r->v[0] = 'G';
    T->r->k[1] = 'M'; T->r->v[1] = 'M';
    T->r->k[2] = 'P'; T->r->v[2] = 'P';
    T->r->k[3] = 'X'; T->r->v[3] = 'X';

    node_t * L1 = allocate_node(3);
    L1->leaf = true; L1->n = 4;
    L1->k[0] = 'A'; L1->v[0] = 'A';
    L1->k[1] = 'C'; L1->v[1] = 'C';
    L1->k[2] = 'D'; L1->v[2] = 'D';
    L1->k[3] = 'E'; L1->v[3] = 'E';
    T->r->c[0] = L1;

    node_t * L2 = allocate_node(3);
    L2->leaf = true; L2->n = 2;
    L2->k[0] = 'J'; L2->v[0] = 'J';
    L2->k[1] = 'K'; L2->v[1] = 'K';
    T->r->c[1] = L2;

    node_t * L3 = allocate_node(3);
    L3->leaf = true; L3->n = 2;
    L3->k[0] = 'N'; L3->v[0] = 'N';
    L3->k[1] = 'O'; L3->v[1] = 'O';
    T->r->c[2] = L3;

    node_t * L4 = allocate_node(3);
    L4->leaf = true; L4->n = 5;
    L4->k[0] = 'R'; L4->v[0] = 'R';
    L4->k[1] = 'S'; L4->v[1] = 'S';
    L4->k[2] = 'T'; L4->v[2] = 'T';
    L4->k[3] = 'U'; L4->v[3] = 'U';
    L4->k[4] = 'V'; L4->v[4] = 'V';
    T->r->c[3] = L4;

    node_t * L5 = allocate_node(3);
    L5->leaf = true; L5->n = 2;
    L5->k[0] = 'Y'; L5->v[0] = 'Y';
    L5->k[1] = 'Z'; L5->v[1] = 'Z';
    T->r->c[4] = L5;

    // Figure 18.7a: Ensure that the manually-constructed initial tree is valid.
    assert_tree_valid(T);

    // Figure 18.7b: Insert 'B' and ensure the tree is valid and matches 18.7b.
    btree_insert(T, 'B', 'B');
    assert_tree_valid(T);
    munit_assert_char(T->r->c[0]->n, ==, 5);
    munit_assert_char(T->r->c[0]->k[0], ==, 'A'); munit_assert_char(T->r->c[0]->v[0], ==, 'A');
    munit_assert_char(T->r->c[0]->k[1], ==, 'B'); munit_assert_char(T->r->c[0]->v[1], ==, 'B');
    munit_assert_char(T->r->c[0]->k[2], ==, 'C'); munit_assert_char(T->r->c[0]->v[2], ==, 'C');
    munit_assert_char(T->r->c[0]->k[3], ==, 'D'); munit_assert_char(T->r->c[0]->v[3], ==, 'D');
    munit_assert_char(T->r->c[0]->k[4], ==, 'E'); munit_assert_char(T->r->c[0]->v[4], ==, 'E');

    // Figure 18.7c: Insert 'Q' and ensure the tree is valid and matches 18.7c.
    btree_insert(T, 'Q', 'Q');
    assert_tree_valid(T);
    munit_assert_uint16(T->r->n, ==, 5);
    munit_assert_char(T->r->k[0], ==, 'G'); munit_assert_char(T->r->v[0], ==, 'G');
    munit_assert_char(T->r->k[1], ==, 'M'); munit_assert_char(T->r->v[1], ==, 'M');
    munit_assert_char(T->r->k[2], ==, 'P'); munit_assert_char(T->r->v[2], ==, 'P');
    munit_assert_char(T->r->k[3], ==, 'T'); munit_assert_char(T->r->v[3], ==, 'T');
    munit_assert_char(T->r->k[4], ==, 'X'); munit_assert_char(T->r->v[4], ==, 'X');
    munit_assert_uint16(T->r->c[3]->n, ==, 3);
    munit_assert_char(T->r->c[3]->k[0], ==, 'Q'); munit_assert_char(T->r->c[3]->v[0], ==, 'Q');
    munit_assert_char(T->r->c[3]->k[1], ==, 'R'); munit_assert_char(T->r->c[3]->v[1], ==, 'R');
    munit_assert_char(T->r->c[3]->k[2], ==, 'S'); munit_assert_char(T->r->c[3]->v[2], ==, 'S');
    munit_assert_uint16(T->r->c[4]->n, ==, 2);
    munit_assert_char(T->r->c[4]->k[0], ==, 'U'); munit_assert_char(T->r->c[4]->v[0], ==, 'U');
    munit_assert_char(T->r->c[4]->k[1], ==, 'V'); munit_assert_char(T->r->c[4]->v[1], ==, 'V');

    // Figure 18.7d: Insert 'L' and ensure the tree is valid and matches 18.7d.
    btree_insert(T, 'L', 'L');
    assert_tree_valid(T);
    munit_assert_uint16(T->r->n, ==, 1);
    munit_assert_char(T->r->k[0], ==, 'P'); munit_assert_char(T->r->v[0], ==, 'P');
    munit_assert_uint16(T->r->c[0]->n, ==, 2);
    munit_assert_char(T->r->c[0]->k[0], ==, 'G'); munit_assert_char(T->r->c[0]->v[0], ==, 'G');
    munit_assert_char(T->r->c[0]->k[1], ==, 'M'); munit_assert_char(T->r->c[0]->v[1], ==, 'M');
    munit_assert_char(T->r->c[0]->c[0]->n, ==, 5);
    munit_assert_char(T->r->c[0]->c[0]->k[0], ==, 'A'); munit_assert_char(T->r->c[0]->c[0]->v[0], ==, 'A');
    munit_assert_char(T->r->c[0]->c[0]->k[1], ==, 'B'); munit_assert_char(T->r->c[0]->c[0]->v[1], ==, 'B');
    munit_assert_char(T->r->c[0]->c[0]->k[2], ==, 'C'); munit_assert_char(T->r->c[0]->c[0]->v[2], ==, 'C');
    munit_assert_char(T->r->c[0]->c[0]->k[3], ==, 'D'); munit_assert_char(T->r->c[0]->c[0]->v[3], ==, 'D');
    munit_assert_char(T->r->c[0]->c[0]->k[4], ==, 'E'); munit_assert_char(T->r->c[0]->c[0]->v[4], ==, 'E');
    munit_assert_char(T->r->c[0]->c[1]->n, ==, 3);
    munit_assert_char(T->r->c[0]->c[1]->k[0], ==, 'J'); munit_assert_char(T->r->c[0]->c[1]->v[0], ==, 'J');
    munit_assert_char(T->r->c[0]->c[1]->k[1], ==, 'K'); munit_assert_char(T->r->c[0]->c[1]->v[1], ==, 'K');
    munit_assert_char(T->r->c[0]->c[1]->k[2], ==, 'L'); munit_assert_char(T->r->c[0]->c[1]->v[2], ==, 'L');
    munit_assert_char(T->r->c[0]->c[2]->n, ==, 2);
    munit_assert_char(T->r->c[0]->c[2]->k[0], ==, 'N'); munit_assert_char(T->r->c[0]->c[2]->v[0], ==, 'N');
    munit_assert_char(T->r->c[0]->c[2]->k[1], ==, 'O'); munit_assert_char(T->r->c[0]->c[2]->v[1], ==, 'O');
    munit_assert_uint16(T->r->c[1]->n, ==, 2);
    munit_assert_char(T->r->c[1]->k[0], ==, 'T'); munit_assert_char(T->r->c[1]->v[0], ==, 'T');
    munit_assert_char(T->r->c[1]->k[1], ==, 'X'); munit_assert_char(T->r->c[1]->v[1], ==, 'X');
    munit_assert_char(T->r->c[1]->c[0]->n, ==, 3);
    munit_assert_char(T->r->c[1]->c[0]->k[0], ==, 'Q'); munit_assert_char(T->r->c[1]->c[0]->v[0], ==, 'Q');
    munit_assert_char(T->r->c[1]->c[0]->k[1], ==, 'R'); munit_assert_char(T->r->c[1]->c[0]->v[1], ==, 'R');
    munit_assert_char(T->r->c[1]->c[0]->k[2], ==, 'S'); munit_assert_char(T->r->c[1]->c[0]->v[2], ==, 'S');
    munit_assert_char(T->r->c[1]->c[1]->n, ==, 2);
    munit_assert_char(T->r->c[1]->c[1]->k[0], ==, 'U'); munit_assert_char(T->r->c[1]->c[1]->v[0], ==, 'U');
    munit_assert_char(T->r->c[1]->c[1]->k[1], ==, 'V'); munit_assert_char(T->r->c[1]->c[1]->v[1], ==, 'V');
    munit_assert_char(T->r->c[1]->c[2]->n, ==, 2);
    munit_assert_char(T->r->c[1]->c[2]->k[0], ==, 'Y'); munit_assert_char(T->r->c[1]->c[2]->v[0], ==, 'Y');
    munit_assert_char(T->r->c[1]->c[2]->k[1], ==, 'Z'); munit_assert_char(T->r->c[1]->c[2]->v[1], ==, 'Z');

    // Figure 18.7e: Insert 'F' and ensure the tree is valid and matches 18.7e.
    btree_insert(T, 'F', 'F');
    assert_tree_valid(T);
    munit_assert_uint16(T->r->n, ==, 1);
    munit_assert_char(T->r->k[0], ==, 'P'); munit_assert_char(T->r->v[0], ==, 'P');
    munit_assert_uint16(T->r->c[0]->n, ==, 3);
    munit_assert_char(T->r->c[0]->k[0], ==, 'C'); munit_assert_char(T->r->c[0]->v[0], ==, 'C');
    munit_assert_char(T->r->c[0]->k[1], ==, 'G'); munit_assert_char(T->r->c[0]->v[1], ==, 'G');
    munit_assert_char(T->r->c[0]->k[2], ==, 'M'); munit_assert_char(T->r->c[0]->v[2], ==, 'M');
    munit_assert_char(T->r->c[0]->c[0]->n, ==, 2);
    munit_assert_char(T->r->c[0]->c[0]->k[0], ==, 'A'); munit_assert_char(T->r->c[0]->c[0]->v[0], ==, 'A');
    munit_assert_char(T->r->c[0]->c[0]->k[1], ==, 'B'); munit_assert_char(T->r->c[0]->c[0]->v[1], ==, 'B');
    munit_assert_char(T->r->c[0]->c[1]->n, ==, 3);
    munit_assert_char(T->r->c[0]->c[1]->k[0], ==, 'D'); munit_assert_char(T->r->c[0]->c[1]->v[0], ==, 'D');
    munit_assert_char(T->r->c[0]->c[1]->k[1], ==, 'E'); munit_assert_char(T->r->c[0]->c[1]->v[1], ==, 'E');
    munit_assert_char(T->r->c[0]->c[1]->k[2], ==, 'F'); munit_assert_char(T->r->c[0]->c[1]->v[2], ==, 'F');
    munit_assert_char(T->r->c[0]->c[2]->n, ==, 3);
    munit_assert_char(T->r->c[0]->c[2]->k[0], ==, 'J'); munit_assert_char(T->r->c[0]->c[2]->v[0], ==, 'J');
    munit_assert_char(T->r->c[0]->c[2]->k[1], ==, 'K'); munit_assert_char(T->r->c[0]->c[2]->v[1], ==, 'K');
    munit_assert_char(T->r->c[0]->c[2]->k[2], ==, 'L'); munit_assert_char(T->r->c[0]->c[2]->v[2], ==, 'L');
    munit_assert_char(T->r->c[0]->c[3]->n, ==, 2);
    munit_assert_char(T->r->c[0]->c[3]->k[0], ==, 'N'); munit_assert_char(T->r->c[0]->c[3]->v[0], ==, 'N');
    munit_assert_char(T->r->c[0]->c[3]->k[1], ==, 'O'); munit_assert_char(T->r->c[0]->c[3]->v[1], ==, 'O');
    munit_assert_uint16(T->r->c[1]->n, ==, 2);
    munit_assert_char(T->r->c[1]->k[0], ==, 'T'); munit_assert_char(T->r->c[1]->v[0], ==, 'T');
    munit_assert_char(T->r->c[1]->k[1], ==, 'X'); munit_assert_char(T->r->c[1]->v[1], ==, 'X');
    munit_assert_char(T->r->c[1]->c[0]->n, ==, 3);
    munit_assert_char(T->r->c[1]->c[0]->k[0], ==, 'Q'); munit_assert_char(T->r->c[1]->c[0]->v[0], ==, 'Q');
    munit_assert_char(T->r->c[1]->c[0]->k[1], ==, 'R'); munit_assert_char(T->r->c[1]->c[0]->v[1], ==, 'R');
    munit_assert_char(T->r->c[1]->c[0]->k[2], ==, 'S'); munit_assert_char(T->r->c[1]->c[0]->v[2], ==, 'S');
    munit_assert_char(T->r->c[1]->c[1]->n, ==, 2);
    munit_assert_char(T->r->c[1]->c[1]->k[0], ==, 'U'); munit_assert_char(T->r->c[1]->c[1]->v[0], ==, 'U');
    munit_assert_char(T->r->c[1]->c[1]->k[1], ==, 'V'); munit_assert_char(T->r->c[1]->c[1]->v[1], ==, 'V');
    munit_assert_char(T->r->c[1]->c[2]->n, ==, 2);
    munit_assert_char(T->r->c[1]->c[2]->k[0], ==, 'Y'); munit_assert_char(T->r->c[1]->c[2]->v[0], ==, 'Y');
    munit_assert_char(T->r->c[1]->c[2]->k[1], ==, 'Z'); munit_assert_char(T->r->c[1]->c[2]->v[1], ==, 'Z');

    return MUNIT_OK;
}

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
    assert_tree_valid(T);

    // Insert one more key to cause a split.
    btree_insert(T, k, k);

    // Validate the root is no longer a leaf, and the whole tree is valid.
    munit_assert(!T->r->leaf);
    assert_tree_valid(T);

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
    assert_tree_valid(T);
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
    assert_tree_valid(T);
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
    assert_tree_valid(T);

    return MUNIT_OK;
}
