cmake_minimum_required(VERSION 3.10)
project(ConceptForge)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define shader and asset directories
add_compile_definitions(SHADER_DIR="${PROJECT_SOURCE_DIR}/shaders")
add_compile_definitions(TEXTURE_DIR="${PROJECT_SOURCE_DIR}/assets/textures")
add_compile_definitions(FONT_DIR="${PROJECT_SOURCE_DIR}/fonts")

# Handle Python dev module detection
if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

# Find Python
find_package(Python COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)
message(STATUS "Python version: ${Python_EXECUTABLE}")

# Set default build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
  src
  external/stb_image
  external/IconFont
  external/entt/single_include
)

# Gather source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE NB_SOURCES "nanobind/*.cpp")

# Add external libraries
add_subdirectory(external/glm)
add_subdirectory(external/glfw)
add_subdirectory(external/nanobind)
add_subdirectory(external/bullet)

# Build glad
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Build ImGui
add_library(imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends
  external/glfw/include
)

# Build ImGuizmo
add_library(ImGuizmo STATIC external/ImGuizmo/ImGuizmo.cpp)
target_include_directories(ImGuizmo PUBLIC external/ImGuizmo)
target_link_libraries(ImGuizmo PUBLIC imgui)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
  PRIVATE OpenGL::GL imgui ImGuizmo glad glfw glm BulletDynamics BulletCollision LinearMath
)

# Custom target to run the app
add_custom_target(run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
)

# nanobind module for Python
# nanobind_add_module(concept_forge ${SOURCES} ${NB_SOURCES})
# target_include_directories(concept_forge PRIVATE
#   src
#   external/glad/include
#   external/imgui
#   external/imgui/backends
#   external/glfw/include
#   external/ImGuizmo
#   external/stb_image
#   external/IconFont
# )
# target_link_libraries(concept_forge
#   PRIVATE OpenGL::GL imgui ImGuizmo glad glfw glm
# )
#
# add_custom_command(TARGET concept_forge POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/python-scripts"
#     COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:concept_forge>" "${CMAKE_CURRENT_SOURCE_DIR}/python-scripts/"
#     COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/python-scripts
#             ${Python_EXECUTABLE} -m nanobind.stubgen -m concept_forge
#     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/python-scripts"
#     COMMENT "Copying Python module to python-scripts/ and generating .pyi stubs"
# )

