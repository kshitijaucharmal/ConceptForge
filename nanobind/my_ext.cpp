// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;
using namespace nb::literals;

#include <memory>

#include "setup.hpp"
#include "primitives/entity.hpp"
#include "primitives/cube.hpp"
#include "primitives/uv_sphere.hpp"

using namespace Engine;
using namespace SimObject;

float add(float a, float b){
    return a + b;
}

void AddCube(ConceptForge &forge, float x, float y, float z){
    std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaderProgram);
    cube->Translate(glm::vec3(x, y, z));
    forge.entities.push_back(std::move(cube));
}

void AddUVSphere(ConceptForge &forge, float x, float y, float z){
    std::unique_ptr<UVSphere> sphere = std::make_unique<UVSphere>(forge.shaderProgram);
    sphere->Translate(glm::vec3(x, y, z));
    forge.entities.push_back(std::move(sphere));
}

NB_MODULE(concept_forge, m) {
    m.def("add", &add, "a"_a, "b"_a, "Adds two numbers");

    //nb::class_<SimObject::Entity, std::shared_ptr<SimObject::Entity>>(m, "Entity");

    nb::class_<ConceptForge>(m, "ConceptForge")
        .def(nb::init<>())
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

    // For testing
    m.def("add_cube", &AddCube);
    m.def("add_uv_sphere", &AddUVSphere);
}
