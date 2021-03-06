#include "../subprojects/munit/munit.h"
#include "create.h"
#include "delete.h"
#include "destroy.h"
#include "insert.h"
#include "search.h"

MunitSuite sub_suites[] = { 
    { "/create", create_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/delete", delete_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/destroy", destroy_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/insert", insert_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/search", search_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE } 
};
static const MunitSuite suite = { "/btree", NULL, sub_suites, 100, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char * const argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}