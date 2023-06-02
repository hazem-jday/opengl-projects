cmake_minimum_required(VERSION 3.0.0)
project(opengl VERSION 0.1.0)


find_package(OpenGL REQUIRED)

add_executable(opengl
    src/main.cpp
    src/glad.c
    src/glad.h
    src/khrplatform.h
    src/stb_image.h
    src/shader_s.h
    src/minaudio.h
)

target_link_libraries(opengl glfw)