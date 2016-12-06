cmake_minimum_required (VERSION 2.8.11)
project (Pic)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions")

option(BuildSubmodules "BuildSubmodules" OFF)
option(AddTests "AddTests" ON)

if(BuildSubmodules)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include ${CMAKE_CURRENT_SOURCE_DIR}/Submodules/Stick)
else()
    include_directories (${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include ${CMAKE_INSTALL_PREFIX}/include)
endif()

link_directories(/usr/local/lib)

set (PICDEPS Stick freeimage pthread)

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

if(BuildSubmodules)
    set(PrevAddTests ${AddTests})
    set(AddTests OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Submodules/Stick)
    set(AddTests ${PrevAddTests})
endif()

add_library (Pic SHARED ${PICSRC})
add_library (PicStatic STATIC ${PICSRC})
target_link_libraries(Pic ${PICDEPS})
target_link_libraries(PicStatic ${PICDEPS})
install (TARGETS Pic PicStatic DESTINATION /usr/local/lib)
foreach ( file ${PICINC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${dir} )
endforeach()

if(AddTests)
    add_subdirectory (Tests)
endif()
