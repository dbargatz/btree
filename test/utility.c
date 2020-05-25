#include "utility.h"
#include "../src/btree.h"

// TODO: add invariants from CLRS to assert_tree_valid
// TODO: fix rand_uint64 to be fast and not hacky.

void assert_node_valid(const node_t * x, bool isRoot) {
    int i;
    node_t * y;
    node_t * z;

    munit_assert_ptr_not_null(x);

    // Validate the min degree.
    munit_assert_uint16(x->t, >=, MIN_DEGREE_MIN);
    munit_assert_uint16(x->t, <=, MIN_DEGREE_MAX);

    // Validate the invariants on number of keys per node.
    if(!isRoot) {
        munit_assert_uint16(x->n, >=, x->t-1);
    }
    munit_assert_uint16(x->n, <=, (2*x->t)-1);

    // Loop through all the keys, values, and recurse on the children.
    for(i = 0; i < x->n; i++) {
        // Ensure the key and value are valid.
        munit_assert_uint64(x->k[i], !=, INVALID_SENTINEL);
        munit_assert_uint64(x->v[i], !=, INVALID_SENTINEL);

        if(i < x->n-1) {
            // Departure from CLRS: each key k[i] must be strictly less-than
            // k[i+1]. Duplicate keys are not allowed, and duplicate inserts 
            // should overwrite the existing value.
            munit_assert_uint64(x->k[i], <, x->k[i+1]);
        }

        y = x->c[i];
        z = x->c[i+1];
        if(x->leaf) {
            // All child pointers in leaves must be NULL.
            munit_assert_ptr_null(y);
            munit_assert_ptr_null(z);
        } else {
            munit_assert_ptr_not_null(y);
            munit_assert_ptr_not_null(z);

            // Validate that the largest key in x->k[i]'s left child is smaller
            // than x->k[i], and the smallest key in x->k[i]'s right child is
            // larger than x->k[i].
            munit_assert_uint64(y->k[y->n-1], <, x->k[i]);
            munit_assert_uint64(z->k[0], >, x->k[i]);

            assert_node_valid(y, false);
            assert_node_valid(z, false);
        }
    }

    // Ensure the remaining keys, values, and children are correctly set as
    // INVALID_SENTINEL or NULL.
    for(i = x->n; i < (2*x->t)-1; i++) {
        munit_assert_uint64(x->k[i], ==, INVALID_SENTINEL);
        munit_assert_uint64(x->v[i], ==, INVALID_SENTINEL);
        munit_assert_ptr_null(x->c[i+1]);
    }
}

void assert_search_failed(btree_search_result_t inResult) {
    munit_assert_ptr_null(inResult.x);
    munit_assert_uint16(inResult.i, ==, 0);
}

void assert_tree_valid(const btree_t * T) {
    munit_assert_ptr_not_null(T);
    munit_assert_uint16(T->t, >=, MIN_DEGREE_MIN);
    munit_assert_uint16(T->t, <=, MIN_DEGREE_MAX);
    assert_node_valid(T->r, true);
}

btree_t * create_populated_tree(uint16_t t, int32_t inNumInserts) {
    // Create a B-tree and fill it with the desired number of random keys,
    // excluding the test key range.
    int i;
    uint64_t k = INVALID_SENTINEL;
    btree_t * T = btree_create(t);
    for(i = 0; i < inNumInserts; i++) {
        k = rand_uint64(RANDOM_KEY_MIN, RANDOM_KEY_MAX);
        btree_insert(T, k, k);
    }

    return T;
}

uint64_t rand_uint64(uint64_t inMin, uint64_t inMax) {
    uint64_t k;
    munit_rand_memory(sizeof(k), (uint8_t *)&k);
    do {
        if(k > inMax) {
            k = k >> 2;
        }
    } while(k < inMin || k > inMax || k == INVALID_SENTINEL);
    return k;
}

void * setup_large(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree with a relatively small min degree to exercise 
    // functionality, but not take forever. Insert enough keys to cause multiple
    // splits.
    uint16_t t = (uint16_t)munit_rand_int_range(DEGREE_MED, MIN_DEGREE_MAX);
    return create_populated_tree(t, munit_rand_int_range(t*4, t*16));
}

void * setup_medium(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree with a relatively small min degree to exercise 
    // functionality, but not take forever. Insert enough keys to cause multiple
    // splits.
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, DEGREE_MED);
    return create_populated_tree(t, munit_rand_int_range(t*4, t*16));
}

void * setup_root(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree and fill it with enough random data to fill the root,
    // but not cause any splits.
    uint16_t t = (uint16_t)munit_rand_int_range(MIN_DEGREE_MIN, DEGREE_MED);
    return create_populated_tree(t, (2*t)-1);
}

void teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}
