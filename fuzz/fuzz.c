#include <stdlib.h>
#include "../src/btree.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  // We use the first two bytes of Data as the degree, then insert the remaining
  // Data as 64-bit key/value pairs in 16-byte chunks (8 bytes key, 8 bytes
  // value). As a result, we need the Size to be at least 18 bytes in length:
  //    - 2 bytes for the degree
  //    - 8 bytes for the key
  //    - 8 bytes for the value
  if(Size < 18) {
      return 0;
  }

  // Create a tree with the degree derived from the first 2 bytes of Data. If
  // btree_create() returns NULL, the degree was likely too large, so skip it.
  uint16_t degree = ((uint16_t *)Data)[0];
  btree_t * tree = btree_create(degree);
  if(tree == NULL) {
      return 0;
  }

  // Move past the degree in Data and update Size to reflect the remaining len.
  Data += 2;
  Size -= 2;

  // Use the remaining data in 16-byte chunks; use the first 8 bytes as the
  // key to insert, and the remaining 8 bytes as the value.
  while(Size >= 16) {
      uint64_t key = ((uint64_t *)Data)[0];
      uint64_t value = ((uint64_t *)Data)[1];
      btree_insert(tree, key, value);
      Data += 16;
      Size -= 16;
  }

  // If this Data didn't cause a crash, clean up memory and exit cleanly.
  btree_destroy(tree);
  return 0;
}
