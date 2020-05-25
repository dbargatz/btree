#ifndef _UTILITY_H
#define _UTILITY_H

#include "../src/btree.h"
#include "../subprojects/munit/munit.h"

#define DEGREE_MED       (500)
#define TEST_KEY_MIN     (0)
#define TEST_KEY_MAX     (UINT32_MAX)
#define RANDOM_KEY_MIN   (UINT32_MAX+1)
#define RANDOM_KEY_MAX   (UINT64_MAX)

void assert_search_failed(btree_search_result_t inResult);
void assert_tree_valid(const btree_t * T);
uint64_t rand_uint64(uint64_t inMin, uint64_t inMax);
void * setup_large(const MunitParameter inParams[], void * inFixture);
void * setup_medium(const MunitParameter inParams[], void * inFixture);
void * setup_root(const MunitParameter inParams[], void * inFixture);
void teardown(void * inFixture);

#endif // _UTILITY_H