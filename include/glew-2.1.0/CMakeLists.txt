cmake_minimum_required(VERSION 3.12)
project(GLEW)

set(GLEW_SOURCE_FILES
    src/glew.c
)

set(GLEW_PUBLIC_HEADERS
    include/GL/glew.h
    include/GL/glxew.h
    include/GL/wglew.h
)

add_library(glew STATIC ${GLEW_SOURCE_FILES} ${GLEW_PUBLIC_HEADERS})

target_include_directories(glew PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
