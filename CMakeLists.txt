cmake_minimum_required(VERSION 3.16)

project(ptcpp)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")

enable_testing()