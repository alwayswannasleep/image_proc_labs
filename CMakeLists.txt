cmake_minimum_required(VERSION 3.7)
project(image_proc_labs)

set(CMAKE_CXX_STANDARD 14)

include_directories(image_io)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/image_io/stb_image.cpp ${CMAKE_SOURCE_DIR}/image_io/stb_image_write.cpp)
add_subdirectory(laba1)
add_subdirectory(laba2)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)