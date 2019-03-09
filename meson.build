project('Pic', 'cpp', default_options : ['cpp_std=c++14'])

if meson.is_subproject() == false
    # disable this bogus warning (should be deprecated in newer clang versions)
    add_global_arguments('-Wno-missing-braces', '-fno-exceptions', language: 'cpp')
else
    message('Building Pic as subproject.')
endif

picInc = [
    'Pic/Channels.hpp',
    'Pic/Image.hpp',
    'Pic/Pixel.hpp',
    'Pic/PixelIterator.hpp'
]
install_headers(picInc, subdir: 'Pic')

# picFreeImageInc = [
#     'Pic/FreeImage/FIImageImpl.hpp',
# ]
# install_headers(picFreeImageInc, subdir: 'Pic/FreeImage')


picSrc = [
    'Pic/Image.cpp'
]

cc = meson.get_compiler('cpp')
if get_option('buildSubmodules') == false
    stickDep = cc.find_library('Stick')
    picDeps = [stickDep, dependency('threads')]
else
    stickProj = subproject('Stick')
    picDeps = [stickProj.get_variable('stickDep')]
endif

if get_option('implementation') == 'freeimage'
    add_project_arguments('-DPIC_IMPLEMENTATION_FREEIMAGE', language: 'cpp')
    picDeps += cc.find_library('freeimage', required: true)
elif get_option('implementation') == 'stb'
    add_project_arguments('-DPIC_IMPLEMENTATION_STB', language: 'cpp')
endif
if host_machine.system() == 'linux'
    picDeps += cc.find_library('dl', required : true)
endif

incDirs = include_directories('.', './Pic/Libs')

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
