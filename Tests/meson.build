tests = executable('PicTests', 'PicTests.cpp', 
    dependencies: picDep, 
    include_directories : incDirs,
    cpp_args : '-fsanitize=address',
    link_args : '-fsanitize=address')
test('Pic Tests', tests, workdir: meson.current_build_dir())