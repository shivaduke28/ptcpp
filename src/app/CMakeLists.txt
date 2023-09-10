add_executable(nee "${CMAKE_CURRENT_SOURCE_DIR}/nee.cpp")
add_executable(volume "${CMAKE_CURRENT_SOURCE_DIR}/volume.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
