#ifndef _UTILITY_H
#define _UTILITY_H

#include "../subprojects/munit/munit.h"

#define TEST_KEY_MIN     (UINT16_MAX+1)
#define TEST_KEY_MAX     (UINT32_MAX)
#define RANDOM_KEY_MIN   (0)
#define RANDOM_KEY_MAX   (UINT64_MAX)

uint64_t rand_uint64(uint64_t inExcludeRangeMin, uint64_t inExcludeRangeMax);
void * setup(const MunitParameter inParams[], void * inFixture);
void teardown(void * inFixture);

#endif // _UTILITY_H