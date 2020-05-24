#include "utility.h"
#include "../src/btree.h"

uint64_t rand_uint64(uint64_t inExcludeRangeMin, uint64_t inExcludeRangeMax) {
    uint64_t key;
    do {
        munit_rand_memory(sizeof(key), (uint8_t *)&key);
    } while((key >= inExcludeRangeMin && key <= inExcludeRangeMax) || key == INVALID_KEY_VALUE_SENTINEL);
    return key;
}

void * setup(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree with a relatively small min degree to exercise search, but 
    // not take forever.
    int i;
    uint64_t key = INVALID_KEY_VALUE_SENTINEL;
    uint16_t t = (uint16_t)munit_rand_int_range(DEGREE_MIN, DEGREE_MAX / 2);
    btree_t * tree = btree_create(t);

    // Generate and insert enough keys to cause multiple splits.
    int32_t numInserts = munit_rand_int_range(tree->t * 4, tree->t * 16);

    // Insert enough keys to cause multiple splits. Exclude the test key range,
    // and use the key as the value.
    for(i = 0; i < numInserts; i++) {
        key = rand_uint64(TEST_KEY_MIN, TEST_KEY_MAX);
        btree_insert(tree, key, key);
    }

    return tree;
}

void teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}
