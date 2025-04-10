cmake_minimum_required(VERSION 3.10..4.0)
project(VulkanTest)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(
  imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_vulkan.cpp)

add_compile_definitions(SHADER_DIR="${PROJECT_SOURCE_DIR}/shaders")

target_include_directories(imgui PUBLIC 
  external/imgui 
  external/imgui/backends)

# Linking Vulkan And GLFW
target_link_libraries(imgui PRIVATE Vulkan::Vulkan glfw)

add_executable(VulkanTest src/main.cpp)
target_link_libraries(VulkanTest PRIVATE imgui)
