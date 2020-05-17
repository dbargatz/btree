#include "../subprojects/munit/munit.h"
#include "../src/btree.h"
#include "create.h"

MunitTest create_tests[] = {
    { "/order_lt_2_fails", order_lt_2_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/order_gte_2_succeeds", order_gte_2_succeeds, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/order_gt_65535_fails", order_gt_65535_fails, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_exists", root_exists, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_leaf", root_is_leaf, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/root_is_empty", root_is_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitSuite sub_suites[] = { 
    { "/create", create_tests, NULL, 100, MUNIT_SUITE_OPTION_NONE }, 
    { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE } 
};
static const MunitSuite suite = { "/btree", NULL, sub_suites, 100, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char * const argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}