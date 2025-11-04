cmake_minimum_required(VERSION 3.15)
project(Renderro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)

find_package(OpenGL REQUIRED)
add_subdirectory(libs/glfw)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_library(GLFW_LIBRARY glfw)
    if(NOT GLFW_LIBRARY)
        message(FATAL_ERROR "GLFW not found")
    endif()
endif()

if(WIN32)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/GLFW)
    find_library(OPENGL_LIBRARY opengl32)
    find_library(GLFW_LIBRARY glfw)
endif()

add_executable(Renderro
    src/main.cpp
    src/Application.cpp
    src/Window.cpp
    src/Shader.cpp
    src/Renderer.cpp
    libs/glad/src/glad.c
)

if(UNIX AND NOT APPLE)
    target_link_libraries(Renderro
        ${GLFW_LIBRARY}
        ${OPENGL_LIBRARY}
        ${X11_LIBRARIES}
        pthread
        dl
    )
endif()

if(WIN32)
    target_link_libraries(Renderro
        ${GLFW_LIBRARY}
        ${OPENGL_LIBRARY}
    )
endif()

file(COPY build/resources/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
