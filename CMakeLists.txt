cmake_minimum_required(VERSION 3.16)

project(ptcpp)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/app")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")

enable_testing()