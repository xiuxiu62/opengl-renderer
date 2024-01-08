cmake_minimum_required(VERSION 3.0)
project(example VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)
set(CMAKE_CXX_COMPILER clang++)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE source src/*.cpp src/*.c src/*.hpp src/*.h)
file(GLOB_RECURSE data res/*)

add_executable(example ${source} ${data})
target_compile_options(example PUBLIC -std=c++17 -Wall -Wfloat-conversion)
target_include_directories(example PUBLIC src)
# file(COPY ${data} DESTINATION res)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPES OFF CACHE BOOL "" FORCE)

add_subdirectory(include/glfw-3.3.9)
# add_subdirectory(include/stb)

target_link_libraries(example PRIVATE 
    glfw 
    OpenGL::GL
    glm::glm)
    # stb::image)
