# Concept Forge - OpenGL Game Engine

## Latest iteration video
https://github.com/user-attachments/assets/5a1f73ab-8473-41c5-b002-9009a24dd1eb


---

## Overview

Concept Forge is a lightweight, modular 3D game engine built from scratch using modern C++20 and OpenGL. It features an embedded Python interpreter with pybind11 bindings, allowing you to control the engine directly from Python scripts.

This project started as an experiment in understanding graphics programming at a fundamental level. What began as a basic OpenGL learning exercise has evolved into a fully-functional engine with shadow mapping, physics integration, hierarchical scene graphs, and an in-editor Python scripting environment.

The ultimate goal of this project is to create an AI-controllable game engine where natural language instructions can drive game logic, simulation, and content creation. The Python integration is the first step toward that vision.

---

## Features

### Rendering
- Multi-pass rendering pipeline (shadow maps, picking, main pass)
- Physically-based lighting with point and directional lights
- Shadow mapping with PCF (Percentage Closer Filtering) for soft shadows
- Multiple shader types: Lit, Toon, Unlit
- Skybox/Cubemap support
- Object picking via color buffer selection
- GPU performance profiling

### Scene Management
- Hierarchical scene graph with parent-child relationships
- World and local transform separation with quaternion rotation
- Entity Component System (ECS) architecture using EnTT

### Editor
- In-editor ImGui-based UI with:
  - Hierarchy window for scene tree navigation
  - Inspector for component editing
  - Integrated Python code editor
- ImGuizmo integration for transform manipulation
- Runtime selection and manipulation of objects

### Physics
- Bullet Physics integration for rigid body simulation
- Sync between physics world and visual transforms

### Scripting
- Embedded Python interpreter
- pybind11-based C++ to Python bindings
- Execute Python code at runtime from the editor
- Access and modify scene entities from Python

### Model Loading
- ASSIMP integration for loading 3D models
- Supports common formats (OBJ, GLTF, etc.)

---

## Building on Linux

### Prerequisites

You will need the following dependencies installed on your system:

- CMake >= 3.10
- Ninja build system
- Python 3.10+ (development files)
- OpenGL development files
- GLFW development files

On Ubuntu/Debian:
```bash
sudo apt install cmake ninja-build python3-dev libpython3-dev libgl-dev libglfw3-dev
```

### Clone and Build

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/kshitijaucharmal/ConceptForge
cd ConceptForge

# Create build directory
mkdir build
cd build

# Configure with Debug mode using Ninja
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..

# Build
cmake --build --target ConceptForge -j$(nproc)

# Run the engine
./concept_forge
```

### Build Options

- `-DCMAKE_BUILD_TYPE=Debug` - Debug build (recommended for development)
- `-DCMAKE_BUILD_TYPE=Release` - Release build (optimized)
- `-DPython_EXECUTABLE=$(which python3)` - Specify Python interpreter (takes .venv by default)

---

## Project Structure

```
ConceptForge/
├── src/
│   ├── Core/              # Core systems (Window, GUI, Physics, Events)
│   ├── Components/       # ECS components (Transform, Mesh, Material, Light)
│   ├── Systems/           # Engine systems (Rendering, Input, Python bindings)
│   │   ├── Rendering/    # Render pipeline, shaders, materials
│   │   ├── Primitives/   # Built-in shapes (Cube, Sphere, Grid)
│   │   └── PythonBinding/ # pybind11 bindings
│   └── app.cpp           # Main application entry point
├── shaders/               # GLSL shader files
├── assets/
│   ├── textures/         # Texture files including cubemaps
│   └── models/          # 3D model files
├── fonts/               # Font files for UI
└── external/            # Submodule dependencies
    ├── entt/            # ECS library
    ├── glfw/            # Windowing
    ├── glm/             # Math library
    ├── assimp/          # Model loading
    ├── bullet/          # Physics
    └── imgui/           # UI framework
```

---

## Architecture

### Entity Component System

The engine uses EnTT for ECS architecture. Entities are identified by `entt::entity` handles, and components are attached directly to them. The scene graph is implemented through the Transform component, which stores parent-child relationships using a linked-list structure.

### Rendering Pipeline

1. **Shadow Pass** - Renders depth map from light's perspective
2. **Picking Pass** - Renders color-coded scene for object selection
3. **Main Pass** - Renders scene with lighting, materials, and effects to framebuffer
4. **UI Pass** - Renders ImGui editor overlays
5. **Screen Pass** - Blits final framebuffer to screen

### Event System

The engine uses a queue-based event system with four update phases:
- `AwakeQueue` - One-time initialization
- `UpdateQueue` - Per-frame logic
- `LateUpdateQueue` - Post-update processing
- `DeleteQueue` - Deferred entity destruction

---

## Recent Changes (February 2026)

- Python/pybind11 integration with runtime code execution
- Shadow mapping with PCF filtering
- Skybox/cubemap rendering with gamma correction
- Inspector system for component editing
- Parent-child hierarchical scene graph
- Object picking for runtime selection
- Material system with configurable shaders
- Toon shading support

---

## Technology Stack

- **Language**: C++20
- **Graphics**: OpenGL 4.5+
- **Build System**: CMake + Ninja
- **UI**: ImGui
- **ECS**: EnTT
- **Physics**: Bullet
- **Python**: pybind11
- **Math**: GLM
- **Model Loading**: ASSIMP

---

## License

MIT License. See LICENSE file for details.

---

## Contact

Created by Kshitij Aucharmal
- Twitter: [@ma_at_anubis](https://x.com/ma_at_anubis)
- GitHub: [kshitijaucharmal](https://github.com/kshitijaucharmal)
