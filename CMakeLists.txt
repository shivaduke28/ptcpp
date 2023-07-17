cmake_minimum_required(VERSION 3.16)

project(ptcpp)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

# 実行可能ファイルをビルド
add_executable(ptcpp ${SOURCES})

enable_testing()
