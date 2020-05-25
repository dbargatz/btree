#include "utility.h"
#include "../src/btree.h"

// TODO: add invariants from CLRS to assert_tree_valid
// TODO: fix rand_uint64 to be fast and not hacky.

void assert_search_failed(btree_search_result_t inResult) {
    munit_assert_ptr_null(inResult.x);
    munit_assert_uint16(inResult.i, ==, 0);
}

void assert_tree_valid(node_t * x) {
    int i;
    node_t * y;
    node_t * z;

    munit_assert_not_null(x);

    for(i = 0; i < x->n; i++) {
        if(i < x->n - 1) {
            munit_assert_uint64(x->k[i], <, x->k[i+1]);
        }

        y = x->c[i];
        z = x->c[i+1];
        if(x->leaf) {
            munit_assert_null(y);
            munit_assert_null(z);
        } else {
            munit_assert_uint64(y->k[y->n - 1], <, x->k[i]);
            munit_assert_uint64(z->k[0], >, x->k[i]);

            assert_tree_valid(y);
            assert_tree_valid(z);
        }
    }
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
