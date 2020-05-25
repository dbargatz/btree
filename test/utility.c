#include "utility.h"
#include "../src/btree.h"

void assert_tree_valid(node_t * inNode) {
    int i;
    node_t * left;
    node_t * right;

    munit_assert_not_null(inNode);

    for(i = 0; i < inNode->n; i++) {
        if(i < inNode->n - 1) {
            munit_assert_uint64(inNode->k[i], <, inNode->k[i+1]);
        }

        left = inNode->c[i];
        right = inNode->c[i+1];
        if(inNode->leaf) {
            munit_assert_null(left);
            munit_assert_null(right);
        } else {
            munit_assert_uint64(left->k[left->n - 1], <, inNode->k[i]);
            munit_assert_uint64(right->k[0], >, inNode->k[i]);

            assert_tree_valid(left);
            assert_tree_valid(right);
        }
    }
}

btree_t * create_populated_tree(uint16_t inMinDegree, int32_t inNumInserts) {
    // Create a B-tree and fill it with the desired number of random keys,
    // excluding the test key range.
    int i;
    uint64_t key = INVALID_KEY_VALUE_SENTINEL;
    btree_t * tree = btree_create(inMinDegree);
    for(i = 0; i < inNumInserts; i++) {
        key = rand_uint64(RANDOM_KEY_MIN, RANDOM_KEY_MAX);
        btree_insert(tree, key, key);
    }

    return tree;
}

uint64_t rand_uint64(uint64_t inMin, uint64_t inMax) {
    uint64_t key;
    munit_rand_memory(sizeof(key), (uint8_t *)&key);
    do {
        if(key > inMax) {
            key = key >> 2;
        }
    } while(key < inMin || key > inMax || key == INVALID_KEY_VALUE_SENTINEL);
    return key;
}

void * setup_large(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree with a relatively small min degree to exercise 
    // functionality, but not take forever. Insert enough keys to cause multiple
    // splits.
    uint16_t t = (uint16_t)munit_rand_int_range(DEGREE_MED, DEGREE_MAX / 2);
    return create_populated_tree(t, munit_rand_int_range(t * 4, t * 16));
}

void * setup_medium(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree with a relatively small min degree to exercise 
    // functionality, but not take forever. Insert enough keys to cause multiple
    // splits.
    uint16_t t = (uint16_t)munit_rand_int_range(DEGREE_MIN, DEGREE_MED);
    return create_populated_tree(t, munit_rand_int_range(t * 4, t * 16));
}

void * setup_root(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree and fill it with enough random data to fill the root,
    // but not cause any splits.
    uint16_t t = (uint16_t)munit_rand_int_range(DEGREE_MIN, DEGREE_MED);
    return create_populated_tree(t, (2 * t)-1);
}

void teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}
