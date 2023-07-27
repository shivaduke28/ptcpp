file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit-tests ${TEST_SOURCES})
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/googletest" "googletest_build")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

target_link_libraries(unit-tests gtest gtest_main)

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")

add_test(NAME TestSuite COMMAND unit-tests)
