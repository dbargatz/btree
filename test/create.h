#include "../subprojects/munit/munit.h"

MunitResult order_lt_2_fails(const MunitParameter inParams[], void * inFixture);
MunitResult order_gte_2_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult order_gt_65535_fails(const MunitParameter inParams[], void * inFixture);
MunitResult root_exists(const MunitParameter inParams[], void * inFixture);
MunitResult root_is_leaf(const MunitParameter inParams[], void * inFixture);
MunitResult root_is_empty(const MunitParameter inParams[], void * inFixture);
