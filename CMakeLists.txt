cmake_minimum_required (VERSION 2.8.11)
project (Pic)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions")
include_directories (${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include)
link_directories(/usr/local/lib)

set (PICDEPS Stick freeimage)

set (PICINC 
Pic/Channels.hpp
Pic/Image.hpp
Pic/Pixel.hpp
Pic/PixelIterator.hpp
)

set (PICSRC
Pic/Image.cpp 
Pic/FreeImage/FIImageImpl.cpp
)

add_library (Pic SHARED ${PICSRC})
add_library (PicStatic STATIC ${PICSRC})
target_link_libraries(Pic ${PICDEPS})
target_link_libraries(PicStatic ${PICDEPS})
install (TARGETS Pic PicStatic DESTINATION /usr/local/lib)
foreach ( file ${PICINC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${dir} )
endforeach()

add_subdirectory (Tests)
