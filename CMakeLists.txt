cmake_minimum_required(VERSION 3.10..)
project(OpenGL)

add_compile_definitions(SHADER_DIR="${PROJECT_SOURCE_DIR}/shaders")
add_compile_definitions(TEXTURE_DIR="${PROJECT_SOURCE_DIR}/assets/textures")
add_compile_definitions(FONT_DIR="${PROJECT_SOURCE_DIR}/fonts")

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

find_package(Python COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_package(OpenGL REQUIRED)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(external/stb_image)
include_directories(external/IconFont)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

# Nano bind
file(GLOB_RECURSE NB_SOURCES "nanobind/*.cpp")

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

target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends
  external/glfw/include)

target_link_libraries(ImGuizmo imgui)

target_include_directories(ImGuizmo PUBLIC external/ImGuizmo)
target_include_directories(glad PUBLIC external/glad/include)

add_subdirectory(external/nanobind)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)

nanobind_add_module(concept_forge ${NB_SOURCES})

# Link Everything to nanobind as well
target_include_directories(concept_forge PRIVATE
  src
  external/glad/include
  external/imgui
  external/imgui/backends
  external/glfw/include
  external/ImGuizmo
)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL imgui ImGuizmo glad glfw glm)

