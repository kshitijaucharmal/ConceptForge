// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/shared_ptr.h>
// For binding glm vectors
#include <glm/glm.hpp>
#include <nanobind/stl/array.h>

namespace nb = nanobind;
using namespace nb::literals;

#include <memory>

#include "primitives/cube.hpp"
#include "primitives/uv_sphere.hpp"
#include "setup.hpp"

using namespace Engine;
using namespace SimObject;

// -------------------------------------------------------------------------
// First arg acts as self
Cube* AddCube(ConceptForge &forge, glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale) {
  std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaderProgram);
  cube->SetPosition(pos);
  cube->SetRotation(rot);
  cube->SetScale(scale);

  // Push to entities and return pointer
  Cube* cube_ptr = cube.get();
  forge.entities.push_back(std::move(cube));
  return cube_ptr;
}

UVSphere* AddUVSphere(ConceptForge &forge, glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale) {
  std::unique_ptr<UVSphere> sphere = std::make_unique<UVSphere>(forge.shaderProgram);
  sphere->SetPosition(pos);
  sphere->SetRotation(rot);
  sphere->SetScale(scale);

  // Push to entities and return pointer
  UVSphere* sphere_ptr = sphere.get();
  forge.entities.push_back(std::move(sphere));
  return sphere_ptr;
}
// -------------------------------------------------------------------------

NB_MAKE_OPAQUE(std::vector<std::shared_ptr<SimObject::Entity>>);

NB_MODULE(concept_forge, m) {
  // Bind Vec3
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

  nb::class_<Entity>(m, "Entity")
    .def(nb::init<>(), "Base Class for Entity with transform information")
    // Getters
    .def("get_position", &Entity::GetPosition, "Get the Position as a Vec3")
    .def("get_rotation", &Entity::GetRotation, "Get the Rotation as a Vec3 (eulerAngles)")
    .def("get_scale", &Entity::GetScale, "Get the Scale as a Vec3")
    // Setters
    .def("set_position", &Entity::SetPosition, "position"_a, "Set the position to a Vec3")
    .def("set_rotation", &Entity::SetRotation, "rotation"_a, "Set the rotation to a Vec3 (eulerAngles)")
    .def("set_scale", &Entity::SetScale, "scale"_a, "Set the scale to a Vec3")
    // // Functions
    .def("translate", &Entity::Translate, "delta"_a, "Change the position by Vec3")
    // Overloaded rotate
    .def("rotate", static_cast<void (Entity::*)(glm::vec3)>(&Entity::Rotate), "delta"_a, "Change the rotation by Vec3")
    .def("rotate", static_cast<void (Entity::*)(float, glm::vec3)>(&Entity::Rotate), "angle"_a, "axis"_a, "Change the rotation by angle and axis")
    .def("scale", &Entity::Scale, "deltaFactor"_a, "Change the scale by Vec3");

  nb::class_<Cube, Entity>(m, "Cube")
    .def(nb::init<ShaderManagement::ShaderProgram&>(), "Represents a Cube Entity");

  nb::class_<UVSphere, Entity>(m, "UVSphere")
    .def(nb::init<ShaderManagement::ShaderProgram&>(), "Represents a UV Coordinate Sphere Entity");

  nb::bind_vector<std::vector<std::shared_ptr<SimObject::Entity>>>(m, "EntityVector");

  nb::class_<ConceptForge>(m, "ConceptForge")
      .def(nb::init<>())
      // In Order
      .def("window_should_close", &ConceptForge::WindowShouldClose,
           "Check if window should close")
      .def("calc_delta_time", &ConceptForge::DeltaTimeCalc,
           "Calculate Delta Time")
      .def("process_input", &ConceptForge::ProcessInput, "Process Input")
      .def("render", &ConceptForge::Render, "Clear Screen and Render")
      .def("calc_projection", &ConceptForge::CalcProjection,
           "Calculate the Projection Matrix")
      .def("gui_management", &ConceptForge::GUIManagement,
           "Draw editor windows")

      // Variables / Objects
      .def_rw("window", &ConceptForge::window)
      .def_rw("deltaTime", &ConceptForge::deltaTime)
      .def_rw("shader_pg", &ConceptForge::shaderProgram)
      .def_rw("input_man", &ConceptForge::input)
      .def_rw("entities", &ConceptForge::entities)
      .def("set_selected", &ConceptForge::SetSelected)
      .def("add_cube", &AddCube, nb::rv_policy::reference_internal, "position"_a, "rotation"_a, "scale"_a, "Adds a cube")
      .def("add_uvsphere", &AddUVSphere, nb::rv_policy::reference_internal, "position"_a, "rotation"_a, "scale"_a, "Adds a uv coordinate Sphere");
}
