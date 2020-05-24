#ifndef _SEARCH_H
#define _SEARCH_H

#include "../subprojects/munit/munit.h"

MunitResult null_tree_returns_invalid (const MunitParameter inParams[], void * inFixture);
MunitResult not_found_returns_invalid (const MunitParameter inParams[], void * inFixture);
MunitResult found_returns_expected (const MunitParameter inParams[], void * inFixture);

extern MunitTest search_tests[];

#endif // _SEARCH_H