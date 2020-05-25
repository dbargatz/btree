project('btree', 'c', version: '0.1.0')

munit_dep = dependency('munit', fallback : ['munit', 'munit_dep'])

btree_src = ['src/btree.c']
btree_lib = static_library('btree', btree_src, name_prefix: '', 
                           name_suffix: 'static', pic: true)

test_src = [
    'test/create.c', 'test/delete.c', 'test/destroy.c', 'test/insert.c',
    'test/main.c',   'test/search.c', 'test/utility.c'
]
test_exe = executable('btree_test', test_src, link_with: btree_lib, 
                      dependencies: munit_dep)
test('btree', test_exe, timeout: 60)

doxygen = find_program('doxygen')

doxygen_data = configuration_data()
doxygen_data.set('TOP_SRCDIR', meson.source_root())
doxygen_data.set('TOP_BUILDDIR', meson.build_root())
doxygen_data.set('OUTPUT_DIR', join_paths(meson.build_root(), 'docs'))
doxygen_data.set('README_PATH', join_paths(meson.source_root(), 'README.md'))
doxygen_data.set('VERSION', meson.project_version())
doxygen_data.set('PROJECT_NAME', meson.project_name())

doxyfile = configure_file(input:         'Doxyfile.in',
                          output:        'Doxyfile',
                          configuration: doxygen_data,
                          install:       false)

docs_target = run_target('docs', command: [doxygen, doxyfile])