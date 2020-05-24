#include "search.h"
#include "../src/btree.h"

#define TEST_DEGREE_MAX  (100)
#define TEST_KEY_MIN     (0)
#define TEST_KEY_MAX     (UINT16_MAX)
#define RANDOM_KEY_MIN   (TEST_KEY_MAX+1)
#define RANDOM_KEY_MAX   (UINT64_MAX)

MunitResult null_tree_fails (const MunitParameter inParams[], void * inFixture);
MunitResult null_root_fails (const MunitParameter inParams[], void * inFixture);
MunitResult not_found_fails (const MunitParameter inParams[], void * inFixture);
MunitResult found_succeeds (const MunitParameter inParams[], void * inFixture);

uint64_t search_rand_uint64(uint64_t inMin, uint64_t inMax) 
{
    uint64_t key;
    do {
        munit_rand_memory(sizeof(key), (uint8_t *)&key);
    } while(inMin > key || inMax < key || key == INVALID_KEY_VALUE_SENTINEL);
    return key;
}

void * search_setup(const MunitParameter inParams[], void * inFixture) {
    // Create a B-tree with a relatively small min degree to exercise search, but 
    // not take forever.
    int i;
    uint64_t key = INVALID_KEY_VALUE_SENTINEL;
    uint16_t t = (uint16_t)munit_rand_int_range(DEGREE_MIN, TEST_DEGREE_MAX);
    btree_t * tree = btree_create(t);

    // Generate and insert enough keys to cause multiple splits.
    int32_t numInserts = munit_rand_int_range(tree->t * 4, tree->t * 16);

    // Insert enough keys to cause multiple splits. Exclude the test key range,
    // and use the key as the value.
    for(i = 0; i < numInserts; i++) {
        key = search_rand_uint64(RANDOM_KEY_MIN, RANDOM_KEY_MAX);
        btree_insert(tree, key, key);
    }

    return tree;
}

void search_teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}

MunitTest search_tests[] = {
    { "/null_tree_fails", null_tree_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/null_root_fails", null_root_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/not_found_fails", not_found_fails, search_setup, search_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/found_succeeds", found_succeeds, search_setup, search_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitResult null_tree_fails (const MunitParameter inParams[], void * inFixture) 
{
    uint64_t value = btree_search(NULL, search_rand_uint64(RANDOM_KEY_MIN, RANDOM_KEY_MAX));
    munit_assert_uint64(value, ==, INVALID_KEY_VALUE_SENTINEL);
    return MUNIT_OK;
}

MunitResult null_root_fails (const MunitParameter inParams[], void * inFixture) 
{
    btree_t * tree = (btree_t *)munit_malloc(sizeof(*tree));
    tree->t = (uint16_t)munit_rand_int_range(1, UINT16_MAX);
    tree->r = NULL;
    uint64_t value = btree_search(tree, search_rand_uint64(RANDOM_KEY_MIN, RANDOM_KEY_MAX));
    munit_assert_uint64(value, ==, INVALID_KEY_VALUE_SENTINEL);
    return MUNIT_OK;
}

MunitResult not_found_fails (const MunitParameter inParams[], void * inFixture)
{
    // Generate a test key guaranteed to NOT be in the tree and search for it.
    btree_t * tree = (btree_t *)inFixture;
    uint64_t key = (uint64_t)munit_rand_int_range(TEST_KEY_MIN, TEST_KEY_MAX);
    uint64_t value = btree_search(tree, key);

    // Make sure the key wasn't found. 
    munit_assert_uint64(value, ==, INVALID_KEY_VALUE_SENTINEL);
    return MUNIT_OK;
}

MunitResult found_succeeds (const MunitParameter inParams[], void * inFixture)
{
    // Generate a test key guaranteed to NOT be in the tree and insert it, using
    // the key itself as the value.
    btree_t * tree = (btree_t *)inFixture;
    uint64_t value = INVALID_KEY_VALUE_SENTINEL;
    uint64_t key = (uint64_t)munit_rand_int_range(TEST_KEY_MIN, TEST_KEY_MAX);
    btree_insert(tree, key, key);

    // Search for the key and make sure the returned value equals the key.
    value = btree_search(tree, key);
    munit_assert_uint64(value, ==, key);
    return MUNIT_OK;
}
