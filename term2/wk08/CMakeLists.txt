cmake_minimum_required(VERSION 3.14)
project(model)
add_executable("${PROJECT_NAME}" main.cpp)

# include Penning
add_subdirectory(RayTracing)

message(${PROJECT_SOURCE_DIR})

target_link_libraries("${PROJECT_NAME}" PUBLIC RayTracing)
target_include_directories("${PROJECT_NAME}" PUBLIC
                        "${PROJECT_BINARY_DIR}"
                        "${PROJECT_SOURCE_DIR}/src")

# install binary
install(TARGETS "${PROJECT_NAME}" DESTINATION ".")