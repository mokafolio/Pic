add_executable (PicTests PicTests.cpp)
target_link_libraries(PicTests Pic ${PICDEPS})
add_custom_target(check COMMAND PicTests)
