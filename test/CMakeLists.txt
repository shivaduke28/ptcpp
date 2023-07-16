include_directories( "${PROJECT_SOURCE_DIR}/src" )
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")

add_executable(vec3-test vec3_test.cpp)
add_executable(ray-test ray_test.cpp)
add_executable(image-test image_test.cpp)
add_executable(ppm-test ppm_test.cpp)
add_executable(sphere-test sphere_test.cpp)

add_test(vec3-test ${EXECUTABLE_OUTPUT_PATH}/vec3-test)
add_test(ray-test ${EXECUTABLE_OUTPUT_PATH}/ray-test)
add_test(image-test ${EXECUTABLE_OUTPUT_PATH}/image-test)
add_test(ppm-test ${EXECUTABLE_OUTPUT_PATH}/ppm-test)
add_test(sphere-test ${EXECUTABLE_OUTPUT_PATH}/sphere-test)
