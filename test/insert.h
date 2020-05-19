#ifndef _INSERT_H
#define _INSERT_H

#include "../subprojects/munit/munit.h"

MunitResult non_full_doesnt_split (const MunitParameter inParams[], void * inFixture);
MunitResult full_causes_split (const MunitParameter inParams[], void * inFixture);

extern MunitTest insert_tests[];

#endif // _INSERT_H