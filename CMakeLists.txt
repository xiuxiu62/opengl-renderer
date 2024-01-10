cmake_minimum_required(VERSION 3.0)
project(impossible-soul VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)
# set(CMAKE_CXX_COMPILER clang++)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE source src/*.cpp src/*.c src/*.hpp src/*.h)
file(GLOB_RECURSE data res/*)

add_executable(impossible-soul ${source} ${data})
target_compile_options(impossible-soul PUBLIC -std=c++17 -Wall -Wfloat-conversion)
target_include_directories(impossible-soul PUBLIC src)
# file(COPY ${data} DESTINATION res)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPES OFF CACHE BOOL "" FORCE)

add_subdirectory(include/glew-2.1.0)
add_subdirectory(include/glfw-3.3.9)
add_subdirectory(include/glm)

target_link_libraries(impossible-soul PRIVATE 
    glfw 
    glew
    glm
    OpenGL::GL
)
