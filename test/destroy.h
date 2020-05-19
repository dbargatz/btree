#ifndef _DESTROY_H
#define _DESTROY_H

#include "../subprojects/munit/munit.h"

MunitResult null_tree_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult valid_tree_succeeds(const MunitParameter inParams[], void * inFixture);
MunitResult null_root_succeeds(const MunitParameter inParams[], void * inFixture);

extern MunitTest destroy_tests[];

#endif // _DESTROY_H