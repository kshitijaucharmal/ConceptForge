cmake_minimum_required(VERSION 3.10..4.0)
project(OpenGL)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(
  imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp)

add_library(
  glad STATIC
  external/glad/src/glad.c
)

add_compile_definitions(SHADER_DIR="${PROJECT_SOURCE_DIR}/shaders")

target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends)

target_include_directories(glad PUBLIC
  external/glad/include)

# Linking Vulkan And GLFW
target_link_libraries(imgui PRIVATE OpenGL::GL glad glfw)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_target(RunMyProgram
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui glad)
