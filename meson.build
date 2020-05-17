project('btree', 'c', version: '0.1.0')

src = ['src/btree.c', 'src/node.c']
lib = static_library('btree', src, name_prefix: '', name_suffix: 'static', 
                     pic: true)

munit_dep = dependency('munit', fallback : ['munit', 'munit_dep'])
test = executable('btree_test', 'test/main.c', link_with: lib, dependencies: munit_dep)

test('initial', test)