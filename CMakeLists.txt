cmake_minimum_required(VERSION 3.20)

project(LOGLAttempt90
    VERSION 0.1.0
    DESCRIPTION "A simple attempt at an OpenGL application"
    LANGUAGES CXX
)

add_executable(LOGLAttempt90 
    ./src/main.cpp src/Shader.cpp src/Shader.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(glad/)
target_link_libraries(LOGLAttempt90 PRIVATE glad)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(LOGLAttempt90 PRIVATE fmt::fmt)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(LOGLAttempt90 PRIVATE glfw)

target_include_directories(LOGLAttempt90 PUBLIC
                           "${PROJECT_BINARY_DIR}")
