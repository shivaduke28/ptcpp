file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit-tests ${TEST_SOURCES})
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/googletest" "googletest_build")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

target_link_libraries(unit-tests gtest gtest_main)

add_test(NAME TestSuite COMMAND unit-tests)
