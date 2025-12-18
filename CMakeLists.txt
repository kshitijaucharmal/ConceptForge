cmake_minimum_required(VERSION 3.10)
project(ConceptForge)

# Option for static linking (default ON)
option(BUILD_STATIC "Build fully static executable" ON)

# Global static linking settings FIRST
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

# Set default build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Platform-specific settings
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    if(BUILD_STATIC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,windows -static-libgcc -static-libstdc++")
    endif()
elseif(APPLE)
    # macOS
else()
    # Linux
    if(BUILD_STATIC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
    endif()
endif()

# Find OpenGL early
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

# Global include directories
include_directories(
    src
    external/stb_image
    external/IconFont
    external/entt/single_include
)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE NB_SOURCES "nanobind/*.cpp")

# External libraries with static settings
add_subdirectory(external/glm)

# GLFW - force static
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# Nanobind
add_subdirectory(external/nanobind)

# Assimp - static only
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

# Bullet Physics - static only
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

# Glad
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# ImGui
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp  # Add demo if needed
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC 
    external/imgui 
    external/imgui/backends
    external/glfw/include
)
target_link_libraries(imgui PUBLIC glfw)

# ImGuizmo
add_library(ImGuizmo STATIC external/ImGuizmo/ImGuizmo.cpp)
target_include_directories(ImGuizmo PUBLIC external/ImGuizmo)
target_link_libraries(ImGuizmo PUBLIC imgui)

# === MAIN EXECUTABLE MUST COME BEFORE target_compile_definitions ===
add_executable(${PROJECT_NAME} ${SOURCES})

# NOW define asset paths and compile definitions (target exists)
set(SHADERS_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../shaders")
set(TEXTURES_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets/textures")
set(FONTS_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../fonts")

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    SHADER_DIR="${SHADERS_PATH}"
    TEXTURE_DIR="${TEXTURES_PATH}"
    FONT_DIR="${FONTS_PATH}"
)

# Linking
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    imgui
    ImGuizmo
    glad
    glfw
    glm::glm
    BulletDynamics
    BulletCollision
    LinearMath
    assimp::assimp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    external/bullet/src
    external/assimp/include
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ws2_32
        opengl32
        gdi32
        user32
        kernel32
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    # Linux
    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread
        dl
        X11
        Xxf86vm
        Xrandr
        Xi
    )
endif()

# Copy assets
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/../shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/../fonts
)

# Run target
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${PROJECT_NAME}
    USES_TERMINAL
)

