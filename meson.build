project('btree', 'c')

src = ['src/btree.c', 'src/node.c']
lib = library('btree', src)

executable('btree_test', 'test/main.c', link_with: lib)