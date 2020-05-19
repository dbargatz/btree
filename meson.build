project('btree', 'c', version: '0.1.0')

munit_dep = dependency('munit', fallback : ['munit', 'munit_dep'])

btree_src = ['src/btree.c', 'src/node.c']
btree_lib = static_library('btree', btree_src, name_prefix: '', 
                           name_suffix: 'static', pic: true)

test_src = ['test/create.c', 'test/destroy.c', 'test/insert.c', 'test/main.c']
test_exe = executable('btree_test', test_src, link_with: btree_lib, 
                      dependencies: munit_dep)
test('btree', test_exe)