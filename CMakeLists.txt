cmake_minimum_required(VERSION 3.15)
project(SimpleRenderer)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/Runtime/Core/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/Runtime/Renderer/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/Runtime/RHI/Public
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
    find_library(OPENGL_LIBRARY opengl32)
    # GLFW is built via add_subdirectory, so we use the target name "glfw" directly
endif()

add_executable(SimpleRenderer
    Engine/Source/Runtime/Launch/main.cpp
    Engine/Source/Runtime/Core/Private/Engine/Application.cpp
    Engine/Source/Runtime/Core/Private/Platform/Window.cpp
    Engine/Source/Runtime/Renderer/Private/Shader.cpp
    Engine/Source/Runtime/Renderer/Private/Renderer.cpp
    Engine/Source/Runtime/Renderer/Private/FScene.cpp
    Engine/Source/Runtime/Renderer/Private/FView.cpp
    Engine/Source/Runtime/Renderer/Private/DeferredShadingRenderer.cpp
    Engine/Source/Runtime/Renderer/Private/Passes/FDepthPass.cpp
    Engine/Source/Runtime/Renderer/Private/Passes/FBasePass.cpp
    Engine/Source/Runtime/Renderer/Private/Passes/FLightingPass.cpp
    Engine/Source/Runtime/Renderer/Private/Passes/FPostProcessPass.cpp
    Engine/Source/Runtime/RHI/Private/RHI.cpp
    Engine/Source/Runtime/Core/Private/Core/FRenderThread.cpp
    Engine/Source/Runtime/Core/Private/Engine/FGameThread.cpp
    libs/glad/src/glad.c
)

if(UNIX AND NOT APPLE)
    target_link_libraries(SimpleRenderer
        ${GLFW_LIBRARY}
        ${OPENGL_LIBRARY}
        ${X11_LIBRARIES}
        pthread
        dl
    )
endif()

if(WIN32)
    target_link_libraries(SimpleRenderer
        glfw
        ${OPENGL_LIBRARY}
    )
endif()

# Copy shaders to the same directory as the executable
add_custom_command(TARGET SimpleRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:SimpleRenderer>/shaders
)
