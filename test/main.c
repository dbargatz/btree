#include "../subprojects/munit/munit.h"
#include "../src/btree.h"

static void * test_setup(const MunitParameter inParams[], void * inFixture) {
    return btree_create(4);
}

static void test_teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}

MunitResult test_split(const MunitParameter inParams[], void * inFixture) {
    btree_t * tree = (btree_t *)inFixture;
    btree_insert(tree, 2, 2);
    btree_insert(tree, 4, 4);
    btree_insert(tree, 6, 6);
    btree_insert(tree, 8, 8);
    btree_insert(tree, 10, 10);
    btree_insert(tree, 1, 1);
    btree_insert(tree, 3, 3);
    btree_insert(tree, 5, 5);
    btree_insert(tree, 7, 7);
    btree_insert(tree, 9, 9);
    btree_insert(tree, 11, 11);
    btree_insert(tree, 12, 12);
    btree_insert(tree, 13, 13);
    return MUNIT_OK;
}

MunitTest tests[] = {
    { "/test_split", test_split, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL },
};

static const MunitSuite suite = { "/btree", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char * const argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}