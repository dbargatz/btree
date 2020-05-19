#include "insert.h"
#include "../src/btree.h"

void * setup(const MunitParameter inParams[], void * inFixture);
void teardown(void * inFixture);

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
    for(i = 0; i < inNode->numKeys; i++) {
        if(inShouldBeLeaf && NULL != inNode->children[i]) {
            munit_error("Node was not a leaf");
            return false;
        } else if(!inShouldBeLeaf && NULL == inNode->children[i]) {
            munit_errorf("Expected non-leaf node child[%d] was NULL, despite having %d keys", 
                         i, inNode->numKeys);
            return false;
        } else if(i < inNode->numKeys - 1 && inNode->keys[i] >= inNode->keys[i+1]) {
            munit_errorf("Node[%d] (0x%016"PRIX64") >= Node[%d] (0x%016"PRIX64")", 
                          i, inNode->keys[i], i+1, inNode->keys[i+1]);
            return false;
        }
    }

    // Check the last child.
    if(!inShouldBeLeaf && NULL == inNode->children[inNode->numKeys]) {
        munit_errorf("Expected non-leaf node child[%d] was NULL, despite having %d keys", 
                    i, inNode->numKeys);
        return false;
    }

    // Ensure the unused children are NULL.
    for(i = inNode->numKeys+1; i < inNode->order; i++) {
        if(NULL != inNode->children[i]) {
            munit_errorf("Node child[%d] was not NULL, despite having only %d keys", 
                         i, inNode->numKeys);
            return false;
        }
    }

    return true;
}

void * setup(const MunitParameter inParams[], void * inFixture) {
    uint16_t order = (uint16_t)munit_rand_int_range(2, 65535);
    return btree_create(order);
}

void teardown(void * inFixture) {
    btree_destroy((btree_t *)inFixture);
}

MunitResult non_full_doesnt_split(const MunitParameter inParams[], void * inFixture) {
    int i;
    btree_t * tree = (btree_t *)inFixture;
    uint16_t numInserts = (uint16_t)munit_rand_int_range(2, tree->order-1);

    // Insert a number of keys, but not enough to cause a split.
    for(i = 0; i < numInserts; i++) {
        btree_insert(tree, rand_uint64(), 0);
    }

    // Ensure the root is still a valid leaf.
    if(!node_valid(tree->root, true)) {
        return MUNIT_FAIL;
    }

    return MUNIT_OK;
}

MunitResult full_causes_split(const MunitParameter inParams[], void * inFixture) {
    int i;
    btree_t * tree = (btree_t *)inFixture;

    // Insert enough keys to completely fill the root.
    for(i = 0; i < tree->order-1; i++) {
        btree_insert(tree, rand_uint64(), 0);
    }

    // Ensure the root is still a valid leaf.
    if(!node_valid(tree->root, true)) {
        return MUNIT_FAIL;
    }

    // Insert one more key to cause a split.
    btree_insert(tree, rand_uint64(), 0);

    // Ensure the root is no longer a leaf.
    if(!node_valid(tree->root, false)) {
        return MUNIT_FAIL;
    }

    return MUNIT_OK;
}
