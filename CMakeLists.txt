cmake_minimum_required(VERSION 3.16)

project(ptcpp)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")

enable_testing()


# set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
# file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

# googletestをサブディレクトリから取得
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest" "googletest_build")

# 実行可能ファイルをビルド

# add_executable(ptcpp ${SOURCES})
# add_executable(Foo "${CMAKE_CURRENT_SOURCE_DIR}/test/vec3_test.cpp")

# target_link_libraries(Foo gtest gtest_main)


# add_test(NAME TestSuite COMMAND my-tests)