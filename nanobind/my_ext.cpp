// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
// For binding glm vectors
#include <nanobind/stl/array.h>
#include <glm/glm.hpp>

namespace nb = nanobind;
using namespace nb::literals;

#include <memory>

#include "primitives/cube.hpp"
#include "primitives/uv_sphere.hpp"
#include "setup.hpp"

using namespace Engine;
using namespace SimObject;

#include "b_math.cpp"

// -------------------------------------------------------------------------
// First arg acts as self
void AddCube(
    SimObject::Entity &entity,
    ConceptForge &forge,
    MathBindings::Vec3Wrapper &pos,
    MathBindings::Vec3Wrapper &rot,
    MathBindings::Vec3Wrapper &scale) {

  std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaderProgram);
  cube->Translate(glm::vec3(pos.x, pos.y, pos.z));
  cube->Rotate(glm::vec3(rot.x, rot.y, rot.z));
  cube->Scale(glm::vec3(scale.x, scale.y, scale.z));
  forge.entities.push_back(std::move(cube));
}

void AddUVSphere(SimObject::Entity &entity, ConceptForge &forge, float x, float y, float z) {
  std::unique_ptr<UVSphere> sphere = std::make_unique<UVSphere>(forge.shaderProgram);
  sphere->Translate(glm::vec3(x, y, z));
  forge.entities.push_back(std::move(sphere));
}
// -------------------------------------------------------------------------

NB_MODULE(concept_forge, m) {
  // nb::class_<SimObject::Entity, std::shared_ptr<SimObject::Entity>>(m, "Entity");
  // nb::class_<Cube, std::shared_ptr<Cube>>(m, "Cube");
  // nb::class_<UVSphere, std::shared_ptr<UVSphere>>(m, "UVSphere");

  // Math submodule
  nb::module_ math = m.def_submodule("Math");
  // Bind glm vec3
  MathBindings::math_binder(math);

  nb::class_<ConceptForge>(m, "ConceptForge")
      .def(nb::init<>())
      // In Order
      .def("window_should_close", &ConceptForge::WindowShouldClose, "Check if window should close")
      .def("calc_delta_time", &ConceptForge::DeltaTimeCalc, "Calculate Delta Time")
      .def("process_input", &ConceptForge::ProcessInput, "Process Input")
      .def("render", &ConceptForge::Render, "Clear Screen and Render")
      .def("calc_projection", &ConceptForge::CalcProjection, "Calculate the Projection Matrix")
      .def("gui_management", &ConceptForge::GUIManagement, "Draw editor windows")

      // Variables / Objects
      .def_rw("window", &ConceptForge::window)
      .def_rw("deltaTime", &ConceptForge::deltaTime)
      .def_rw("shader_pg", &ConceptForge::shaderProgram)
      .def_rw("input_man", &ConceptForge::input)
      .def_rw("entities", &ConceptForge::entities)
      .def("set_selected", &ConceptForge::SetSelected);

  nb::class_<SimObject::Entity>(m, "Entity")
      .def(nb::init<>())
      .def("add_cube", &AddCube)
      .def("add_uv_sphere", &AddUVSphere);
}
