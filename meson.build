project('Pic', 'cpp', default_options : ['cpp_std=c++14'])

if meson.is_subproject() == false
    # disable this bogus warning (should be deprecated in newer clang versions)
    add_global_arguments('-Wno-missing-braces', '-fno-exceptions', language: 'cpp')
else
    message('Building Stick as subproject.')
endif


picInc = [
    'Pic/Channels.hpp',
    'Pic/Image.hpp',
    'Pic/Pixel.hpp',
    'Pic/PixelIterator.hpp'
]
install_headers(picInc, subdir: 'Pic')

picFreeImageInc = [
    'Pic/FreeImage/FIImageImpl.hpp',
]
install_headers(picFreeImageInc, subdir: 'Pic/FreeImage')


picSrc = [
    'Pic/Image.cpp',
    'Pic/FreeImage/FIImageImpl.cpp'
]

cc = meson.get_compiler('cpp')
if get_option('buildSubmodules') == false
    stickDep = cc.find_library('Stick')
    picDeps = [stickDep, dependency('threads')]
else
    stickProj = subproject('Stick')
    picDeps = [stickProj.get_variable('stickDep')]
endif

picDeps += cc.find_library('freeImage')

if host_machine.system() == 'linux'
    picDeps += cc.find_library('dl', required : true)
endif

incDirs = include_directories('.')

pic = library('Pic', 
        picSrc,
        dependencies: picDeps, 
        include_directories : incDirs, 
        install: true)

picDep = declare_dependency(link_with : pic, 
    dependencies: picDeps, 
    include_directories: incDirs)

# only build tests if not build as a subproject.
# Otherwise it seems like the test meson function uses the wrong tests???
if meson.is_subproject() == false
    subdir('Tests')
endif
