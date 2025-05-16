cmake_minimum_required(VERSION 3.10..4.0)
project(OpenGL)

find_package(OpenGL REQUIRED)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(external/stb_image)
include_directories(external/IconFont)

add_library(
  imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(
  ImGuizmo STATIC
  external/ImGuizmo/ImGuizmo.cpp
)

add_library(
  glad STATIC
  external/glad/src/glad.c
)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

add_compile_definitions(SHADER_DIR="${PROJECT_SOURCE_DIR}/shaders")
add_compile_definitions(TEXTURE_DIR="${PROJECT_SOURCE_DIR}/assets/textures")
add_compile_definitions(FONT_DIR="${PROJECT_SOURCE_DIR}/fonts")

target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends
  external/glfw/include)

target_include_directories(ImGuizmo PUBLIC external/ImGuizmo)
target_link_libraries(ImGuizmo imgui)

target_include_directories(glad PUBLIC
  external/glad/include)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL imgui ImGuizmo glad glfw glm)
