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
void AddCube(SimObject::Entity &entity, ConceptForge &forge,
             glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale) {

  std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaderProgram);
  cube->SetPosition(pos);
  cube->SetRotation(rot);
  cube->SetScale(scale);
  forge.entities.push_back(std::move(cube));
}

void AddUVSphere(SimObject::Entity &entity, ConceptForge &forge, float x,
                 float y, float z) {
  std::unique_ptr<UVSphere> sphere =
      std::make_unique<UVSphere>(forge.shaderProgram);
  sphere->Translate(glm::vec3(x, y, z));
  forge.entities.push_back(std::move(sphere));
}
// -------------------------------------------------------------------------

NB_MAKE_OPAQUE(std::vector<std::shared_ptr<SimObject::Entity>>);

NB_MODULE(concept_forge, m) {
  // Math submodule
  // nb::module_ math = m.def_submodule("Math");
  // Bind glm vec3
  // MathBindings::math_binder(math);

  nb::class_<glm::vec3>(m, "Vec3")
  .def(nb::init<float, float, float>(), "Constructor with x, y, z")
  .def_rw("x", &glm::vec3::x)
  .def_rw("y", &glm::vec3::y)
  .def_rw("z", &glm::vec3::z)
  .def("__repr__", [](const glm::vec3 &v) {
    return "<Vec3 x=" + std::to_string(v.x) +
    ", y=" + std::to_string(v.y) +
    ", z=" + std::to_string(v.z) + ">";
  });

  nb::class_<SimObject::Entity>(m, "Entity")
      .def(nb::init<>())
      // Getters
      .def("get_position", [](const SimObject::Entity &self) {
        glm::vec3 pos = self.GetPosition();
        return nb::make_tuple(pos.x, pos.y, pos.z);
      })
      .def("get_rotation", [](const SimObject::Entity &self) {
        glm::vec3 rot = self.GetRotation();
        return nb::make_tuple(rot.x, rot.y, rot.z);
      })
      .def("get_scale", [](const SimObject::Entity &self) {
        glm::vec3 scl = self.GetScale();
        return nb::make_tuple(scl.x, scl.y, scl.z);
      })
      // Setters
      .def("set_position", [](SimObject::Entity &self, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values for position");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.SetPosition(glm::vec3(x, y, z));
      })
      .def("set_rotation", [](SimObject::Entity &self, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values for rotation");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.SetRotation(glm::vec3(x, y, z));
      })
      .def("set_scale", [](SimObject::Entity &self, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values for scale");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.SetScale(glm::vec3(x, y, z));
      })
      // Functions
      .def("translate", [](SimObject::Entity &self, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.Translate(glm::vec3(x, y, z));
      })
      .def("rotate", [](SimObject::Entity &self, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.Rotate(glm::vec3(x, y, z));
      })
      .def("rotate", [](SimObject::Entity &self, float axis, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.Rotate(axis, glm::vec3(x, y, z));
      })
      .def("scale", [](SimObject::Entity &self, nb::tuple t) {
        if (nb::len(t) != 3)
          throw std::runtime_error("Expected 3 values");

        float x = nb::cast<float>(t[0]);
        float y = nb::cast<float>(t[1]);
        float z = nb::cast<float>(t[2]);

        self.Scale(glm::vec3(x, y, z));
      })
      .def("add_cube", &AddCube)
      .def("add_uv_sphere", &AddUVSphere);

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
      .def("set_selected", &ConceptForge::SetSelected);

}
