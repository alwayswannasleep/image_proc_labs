cmake_minimum_required(VERSION 3.7)
project(image_proc_labs)

set(CMAKE_CXX_STANDARD 14)

include_directories(image_io)

set(SOURCE_FILES main.cpp image_io/stb_image.cpp image_io/stb_image_write.cpp)
add_executable(image_proc_labs ${SOURCE_FILES})

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)