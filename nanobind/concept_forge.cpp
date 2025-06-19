// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/unordered_map.h>
#include <nanobind/stl/bind_map.h>
#include <nanobind/stl/shared_ptr.h>
// For binding glm vectors
#include <glm/glm.hpp>
#include <nanobind/stl/array.h>

namespace nb = nanobind;
using namespace nb::literals;

#include <memory>
#include <unordered_map>

#include "primitives/cube.hpp"
#include "primitives/uv_sphere.hpp"
#include "shaderman.hpp"
#include "editor/hierarchy.hpp"
#include "setup.hpp"

using namespace Engine;
using namespace SimObject;

// -------------------------------------------------------------------------
// First arg acts as self
Cube* AddCube(ConceptForge &forge, glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale, ShaderType shaderType=ShaderType::Lit) {
  auto shader = forge.shaders[shaderType].get();
  std::unique_ptr<Cube> cube = std::make_unique<Cube>(shader);
  cube->SetPosition(pos);
  cube->SetRotation(rot);
  cube->SetScale(scale);

  // Push to entities and return pointer
  Cube* cube_ptr = cube.get();
  forge.hierarchy.AddEntity(std::move(cube));
  return cube_ptr;
}

UVSphere* AddUVSphere(ConceptForge &forge, glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale) {
  std::unique_ptr<UVSphere> sphere = std::make_unique<UVSphere>(forge.shaders[ShaderType::Lit].get());
  sphere->SetPosition(pos);
  sphere->SetRotation(rot);
  sphere->SetScale(scale);

  // Push to entities and return pointer
  UVSphere* sphere_ptr = sphere.get();
  forge.hierarchy.AddEntity(std::move(sphere));
  return sphere_ptr;
}
// ---------------------------------------------------------------------------------------------------

// Complex types need to be OPAQUE
NB_MAKE_OPAQUE(std::unordered_map<EntityID, std::shared_ptr<SimObject::Entity>>);

NB_MODULE(concept_forge, m) {
    // Submodules ----------------------------------------------------------------------------------------
    nb::module_ primitives = m.def_submodule("primitives", "Primitive 3D objects that derive from Entity");
    // ---------------------------------------------------------------------------------------------------

    // Bind Vec3 -----------------------------------------------------------------------------------------
    nb::class_<glm::vec3>(m, "Vec3", "A 3D vector with float components x, y, and z.")
    .def(nb::init<float, float, float>(),
        "Constructor with x, y, z\n\n"
        "Args:\n"
        "    x (float): X-component\n"
        "    y (float): Y-component\n"
        "    z (float): Z-component")
    .def_rw("x", &glm::vec3::x, "X-component of the vector")
    .def_rw("y", &glm::vec3::y, "Y-component of the vector")
    .def_rw("z", &glm::vec3::z, "Z-component of the vector")

    .def("__repr__", [](const glm::vec3 &v) {
      return "<Vec3 x=" + std::to_string(v.x) +
      ", y=" + std::to_string(v.y) +
      ", z=" + std::to_string(v.z) + ">";
    }, "Return the string representation of the vector in the form '<Vec3 x=X, y=Y, z=Z>'")

    // Arithmetic operators
    .def(nb::self + nb::self, "Vector addition. Returns a new Vec3 which is the sum of this and another Vec3.")
    .def(nb::self - nb::self, "Vector subtraction. Returns a new Vec3 which is the difference between this and another Vec3.")
    .def(nb::self * float(), "Scalar multiplication. Multiplies the vector by a scalar float.")
    .def(float() * nb::self, "Scalar multiplication. Multiplies the vector by a scalar float.")
    .def(nb::self * nb::self, "Component-wise multiplication. Multiplies each corresponding component of two vectors.")

    // Conversion methods
    .def("to_tuple", [](const glm::vec3 &v) {
      return std::make_tuple(v.x, v.y, v.z);
    }, "Convert the vector to a Python tuple of the form (x, y, z).")

    .def("to_list", [](const glm::vec3 &v) {
      return std::vector<float>{v.x, v.y, v.z};
    }, "Convert the vector to a Python list [x, y, z].")

    .def("to_dict", [](const glm::vec3 &v) {
      nb::dict d;
      d["x"] = v.x;
      d["y"] = v.y;
      d["z"] = v.z;
      return d;
    }, "Convert the vector to a Python dictionary {'x': x, 'y': y, 'z': z}.");
    // ---------------------------------------------------------------------------------------------------

    // Shader Manager ------------------------------------------------------------------------------------

    // --- Enum: DrawMode ---
    nb::enum_<DrawMode>(m, "DrawMode", "Rendering mode used by shaders")
    .value("WIREFRAME", DrawMode::WIREFRAME, "Render using wireframe mode (edges only)")
    .value("FILLED", DrawMode::FILLED, "Render using filled (solid) polygons")
    .export_values();

    // --- Enum: ShaderType ---
    nb::enum_<ShaderType>(m, "ShaderType", "Types of built-in shaders used in the engine")
    .value("Unlit", ShaderType::Unlit, "Unlit shader (no lighting calculations)")
    .value("Lit", ShaderType::Lit, "Lit shader (basic lighting enabled)")
    .value("Light", ShaderType::Light, "Light shader used for rendering light sources")
    .export_values();

    // --- Class: ShaderProgram ---
    nb::class_<ShaderProgram>(m, "ShaderProgram", "Encapsulates a GLSL shader program")
    .def(nb::init<>(), "Create an empty ShaderProgram")

    .def("init", &ShaderProgram::Init,
         "Compile, link and initialize the shader program",
         nb::arg("mode"), nb::arg("vertexShaderPath"), nb::arg("fragmentShaderPath"))

    .def("set_draw_mode", &ShaderProgram::SetDrawMode,
         "Set the rendering mode (wireframe or filled)",
         nb::arg("mode"))

    .def("use", &ShaderProgram::Use,
         "Activate this shader program for subsequent draw calls")

    .def("link_shaders", &ShaderProgram::LinkShaders,
         "Link the compiled vertex and fragment shaders into a complete program")

    .def("send_data_to_shader", &ShaderProgram::SendDataToShader,
         "Send additional required uniform data to the shader")

    .def("bind_texture", &ShaderProgram::BindTexture,
         "Bind a texture to the shader and upload it to a uniform sampler",
         nb::arg("texturePath"), nb::arg("textureShaderName"),
         nb::arg("textureLoc"), nb::arg("flip") = true)

    // --- Uniform setters ---
    .def("set_bool", &ShaderProgram::setBool,
         "Set a boolean uniform",
         nb::arg("name"), nb::arg("value"))

    .def("set_int", &ShaderProgram::setInt,
         "Set an integer uniform",
         nb::arg("name"), nb::arg("value"))

    .def("set_float", &ShaderProgram::setFloat,
         "Set a float uniform",
         nb::arg("name"), nb::arg("value"))

    .def("set_vec2", &ShaderProgram::setVec2,
         "Set a vec2 uniform (2D vector)",
         nb::arg("name"), nb::arg("value"))

    .def("set_vec3", &ShaderProgram::setVec3,
         "Set a vec3 uniform (3D vector)",
         nb::arg("name"), nb::arg("value"))

    .def("set_vec4", &ShaderProgram::setVec4,
         "Set a vec4 uniform (4D vector)",
         nb::arg("name"), nb::arg("value"))

    .def("set_mat4", &ShaderProgram::setMat4,
         "Set a mat4 uniform (4x4 matrix)",
         nb::arg("name"), nb::arg("value"));

    // ---------------------------------------------------------------------------------------------------

    // Entity --------------------------------------------------------------------------------------------
    nb::class_<Entity>(m, "Entity", "Base class representing a 3D object with position, rotation, and scale")
    .def(nb::init<>(), "Create a new Entity with default position (0,0,0), rotation (0,0,0), and scale (1,1,1).")

    .def_rw("shader", &Entity::shader)

    // Getters
    .def("get_position", &Entity::GetPosition,
         "Return the current position of the Entity as a Vec3 (x, y, z).")

    .def("get_rotation", &Entity::GetRotation,
         "Return the current rotation of the Entity as a Vec3 in Euler angles (in degrees).")

    .def("get_scale", &Entity::GetScale,
         "Return the current scale of the Entity as a Vec3 (x, y, z).")

    // Setters
    .def("set_position", &Entity::SetPosition, "position"_a,
         "Set the position of the Entity.\n\n"
         "Args:\n"
         "    position (Vec3): The new position vector.")

    .def("set_rotation", &Entity::SetRotation, "rotation"_a,
         "Set the rotation of the Entity using Euler angles in degrees.\n\n"
         "Args:\n"
         "    rotation (Vec3): The new rotation vector.")

    .def("set_scale", &Entity::SetScale, "scale"_a,
         "Set the scale of the Entity.\n\n"
         "Args:\n"
         "    scale (Vec3): The new scale vector.")

    // Transform operations
    .def("translate", &Entity::Translate, "delta"_a,
         "Translate (move) the Entity by a given delta vector.\n\n"
         "Args:\n"
         "    delta (Vec3): The amount to move in each axis.")

    .def("rotate", static_cast<void (Entity::*)(glm::vec3)>(&Entity::Rotate), "delta"_a,
         "Rotate the Entity by a delta rotation in Euler angles (degrees).\n\n"
         "Args:\n"
         "    delta (Vec3): Rotation to apply in each axis.")

    .def("rotate", static_cast<void (Entity::*)(float, glm::vec3)>(&Entity::Rotate), "angle"_a, "axis"_a,
         "Rotate the Entity around a given axis by a specified angle (degrees).\n\n"
         "Args:\n"
         "    angle (float): Angle in degrees.\n"
         "    axis (Vec3): Axis to rotate around.")

    .def("scale", &Entity::Scale, "deltaFactor"_a,
         "Scale the Entity by multiplying its current scale by the given factor.\n\n"
         "Args:\n"
         "    deltaFactor (Vec3): Scale multiplier for each axis.");
  // ---------------------------------------------------------------------------------------------------

  // Primitives ----------------------------------------------------------------------------------------
  nb::class_<Cube, Entity>(primitives, "Cube")
    .def("__repr__", [](const Cube *) { return "<Cube>"; })
    .def(nb::init<ShaderManagement::ShaderProgram*>(), "Represents a Cube Entity")
    .def_static("new", &AddCube, nb::rv_policy::reference,
    "forge"_a, "position"_a, "rotation"_a, "scale"_a, "shader"_a = ShaderType::Lit,
    "Create and register a Cube, returning it.");

  nb::class_<UVSphere, Entity>(primitives, "UVSphere")
    .def("__repr__", [](const UVSphere *) { return "<UVSphere>"; })
    .def(nb::init<ShaderManagement::ShaderProgram*>(), "Represents a UV Coordinate Sphere Entity")
    .def_static("new", &AddUVSphere, nb::rv_policy::reference,
    "forge"_a, "position"_a, "rotation"_a, "scale"_a,
    "Create and register a UVSphere, returning it.");
  // ---------------------------------------------------------------------------------------------------

  // Complex elements ----------------------------------------------------------------------------------
  using EntityMap = std::unordered_map<EntityID, std::shared_ptr<SimObject::Entity>>;

  // This binds EntityMap as a Python-compatible dict-like class
  nb::bind_map<EntityMap>(m, "EntityMap");
  // ---------------------------------------------------------------------------------------------------

  // Hierarchy -----------------------------------------------------------------------------------------
  nb::class_<Editor::Hierarchy>(m, "Hierarchy")
    .def(nb::init<>(), "Create new Hierarchy instance")
    .def_rw("entities", &Editor::Hierarchy::entities);
  // ---------------------------------------------------------------------------------------------------

  // Main Class ----------------------------------------------------------------------------------------
  nb::class_<ConceptForge>(m, "ConceptForge",
                           "The main application context that manages the rendering window, input, GUI, and scene entities.\n"
                           "This class acts as the core engine loop handler and entity manager.")

    // Constructor
    .def(nb::init<>(), "Create a new ConceptForge instance with default settings.")

    // Engine loop and rendering methods
    .def("window_should_close", &ConceptForge::WindowShouldClose,
        "Check if the rendering window should close (e.g. user clicked the close button).\n\n"
        "Returns:\n"
        "    bool: True if the window should close, False otherwise.")

    .def("dt", &ConceptForge::DeltaTimeCalc,
        "Return the time elapsed between the current and previous frame.\n\n"
        "Useful for time-based animation and physics.\n\n"
        "Returns:\n"
        "    float: Delta time in seconds.")

    .def("process_input", &ConceptForge::ProcessInput,
        "Poll and process input events from the window (keyboard, mouse, etc).")

    .def("render", &ConceptForge::Render,
        "Clear the screen and render all registered entities.")

    .def("calc_projection", &ConceptForge::CalcProjection,
        "Recalculate the camera projection matrix based on current parameters like field of view, aspect ratio, etc.")

    .def("gui_management", &ConceptForge::GUIManagement,
        "Render the GUI (e.g. ImGui windows for editor features).")

    // Public members
    .def_rw("window", &ConceptForge::window,
            "The rendering window object associated with the application.")

    .def_rw("input_man", &ConceptForge::input,
            "The input manager handling keyboard and mouse states.")

    .def_rw("hierarchy", &ConceptForge::hierarchy,
            "Hierarchy object that holds references to all entities in the scene");

  // ---------------------------------------------------------------------------------------------------
}
