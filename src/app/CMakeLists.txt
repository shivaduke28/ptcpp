add_executable(ptcpp "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_executable(draw_shadow "${CMAKE_CURRENT_SOURCE_DIR}/draw_shadow.cpp")
add_executable(super_sampling "${CMAKE_CURRENT_SOURCE_DIR}/super_sampling.cpp")
add_executable(montecarlo "${CMAKE_CURRENT_SOURCE_DIR}/montecarlo.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
