#include "../subprojects/munit/munit.h"
#include "create.h"
#include "destroy.h"

MunitSuite sub_suites[] = { 
    { "/create", create_tests, NULL, 100, MUNIT_SUITE_OPTION_NONE },
    { "/destroy", destroy_tests, NULL, 100, MUNIT_SUITE_OPTION_NONE },
    { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE } 
};
static const MunitSuite suite = { "/btree", NULL, sub_suites, 100, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char * const argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}