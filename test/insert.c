#include "insert.h"
#include "../src/btree.h"

MunitResult non_full_doesnt_split (const MunitParameter inParams[], void * inFixture);
MunitResult full_causes_split (const MunitParameter inParams[], void * inFixture);

void * setup(const MunitParameter inParams[], void * inFixture) {
    uint16_t t = (uint16_t)munit_rand_int_range(2, DEGREE_MAX / 2);
    return btree_create(t);
}

void teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}

MunitTest insert_tests[] = {
    { "/non_full_doesnt_split", non_full_doesnt_split, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/full_causes_split", full_causes_split, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

uint64_t rand_uint64(void) {
    uint64_t key;
    munit_rand_memory(sizeof(key), (uint8_t *)&key);
    return key;
}

bool node_valid(node_t * inNode, bool inShouldBeLeaf) {
    int i;
    for(i = 0; i < inNode->n; i++) {
        if(inShouldBeLeaf && NULL != inNode->c[i]) {
            munit_error("Node was not a leaf");
            return false;
        } else if(!inShouldBeLeaf && NULL == inNode->c[i]) {
            munit_errorf("Expected non-leaf node child[%d] was NULL, despite having %d keys", 
                         i, inNode->n);
            return false;
        } else if(i < inNode->n - 1 && inNode->k[i] >= inNode->k[i+1]) {
            munit_errorf("Node[%d] (0x%016"PRIX64") >= Node[%d] (0x%016"PRIX64")", 
                          i, inNode->k[i], i+1, inNode->k[i+1]);
            return false;
        }
    }

    // Check the last child.
    if(!inShouldBeLeaf && NULL == inNode->c[inNode->n]) {
        munit_errorf("Expected non-leaf node child[%d] was NULL, despite having %d keys", 
                    i, inNode->n);
        return false;
    }

    // Ensure the unused children are NULL.
    for(i = inNode->n+1; i < (2 * inNode->t); i++) {
        if(NULL != inNode->c[i]) {
            munit_errorf("Node child[%d] was not NULL, despite having only %d keys", 
                         i, inNode->n);
            return false;
        }
    }

    return true;
}

MunitResult non_full_doesnt_split(const MunitParameter inParams[], void * inFixture) {
    int i;
    btree_t * tree = (btree_t *)inFixture;
    uint16_t numInserts = (uint16_t)munit_rand_int_range(2, (2 * tree->t)-1);

    // Insert a number of keys, but not enough to cause a split.
    for(i = 0; i < numInserts; i++) {
        btree_insert(tree, rand_uint64(), 0);
    }

    // Ensure the root is still a valid leaf.
    if(!node_valid(tree->r, true)) {
        return MUNIT_FAIL;
    }

    return MUNIT_OK;
}

MunitResult full_causes_split(const MunitParameter inParams[], void * inFixture) {
    int i;
    btree_t * tree = (btree_t *)inFixture;

    // Insert enough keys to completely fill the root.
    for(i = 0; i < (2 * tree->t)-1; i++) {
        btree_insert(tree, rand_uint64(), 0);
    }

    // Ensure the root is still a valid leaf.
    if(!node_valid(tree->r, true)) {
        return MUNIT_FAIL;
    }

    // Insert one more key to cause a split.
    btree_insert(tree, rand_uint64(), 0);

    // Ensure the root is no longer a leaf.
    if(!node_valid(tree->r, false)) {
        return MUNIT_FAIL;
    }

    return MUNIT_OK;
}
