cmake_minimum_required(VERSION 3.10)
project(ConceptForge)

option(BUILD_STATIC "Build fully static executable" ON)
option(GLFW_BUILD_WAYLAND "Prefer Wayland over X11" OFF)

# Global static linking
if(BUILD_STATIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    message(STATUS "Building fully static executable")
else()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Platform detection
cmake_policy(SET CMP0072 NEW)

if(WIN32 OR APPLE)
    find_package(OpenGL REQUIRED)
endif()

# === GLFW Configuration ===
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

# Wayland/X11 selection
if(UNIX AND NOT APPLE)
  if(GLFW_BUILD_WAYLAND)
    set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
        message(STATUS "Using Wayland backend")
    else()
      set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
      set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
        message(STATUS "Using X11 backend")
    endif()
endif()

add_subdirectory(external/glfw)

# Rest of libraries...
add_subdirectory(external/glm)
add_subdirectory(external/nanobind)

# Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

# Bullet
set(BULLET_ROOT_DIR ${CMAKE_SOURCE_DIR}/external/bullet)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2 OFF CACHE BOOL "" FORCE)
add_subdirectory(${BULLET_ROOT_DIR})

# Libraries
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(imgui STATIC
    external/imgui/imgui.cpp external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends external/glfw/include)
target_link_libraries(imgui PUBLIC glfw)

add_library(ImGuizmo STATIC external/ImGuizmo/ImGuizmo.cpp)
target_include_directories(ImGuizmo PUBLIC external/ImGuizmo)
target_link_libraries(ImGuizmo PUBLIC imgui)

# Global includes
include_directories(src external/stb_image external/IconFont external/entt/single_include)

# Sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Asset paths
set(SHADERS_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../shaders")
set(TEXTURES_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets/textures")
set(FONTS_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../fonts")
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    SHADER_DIR="${SHADERS_PATH}"
    TEXTURE_DIR="${TEXTURES_PATH}"
    FONT_DIR="${FONTS_PATH}"
)

# Core linking (GLAD handles OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE
    glad imgui ImGuizmo glfw glm::glm
    BulletDynamics BulletCollision LinearMath assimp::assimp
)

target_include_directories(${PROJECT_NAME} PRIVATE external/bullet/src)

# === Platform-specific linking ===
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 gdi32 user32 kernel32 ws2_32)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa" "-framework OpenGL" "-framework IOKit" "-framework CoreVideo")
else()
    # Linux/X11/Wayland - GLFW handles display backend
    if(GLFW_BUILD_WAYLAND)
        target_link_libraries(${PROJECT_NAME} PRIVATE wayland-client wayland-cursor wayland-egl xkbcommon)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE X11 Xxf86vm Xrandr Xi Xinerama Xcursor pthread dl)
    endif()
endif()

# Asset copy
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/../shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/../fonts
)

add_custom_target(run COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} DEPENDS ${PROJECT_NAME} USES_TERMINAL)

