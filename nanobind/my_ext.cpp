// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;
using namespace nb::literals;

#include <memory>

#include "setup.hpp"
#include "primitives/cube.hpp"
using namespace Engine;

float add(float a, float b){
    return a + b;
}

void AddCube(ConceptForge &forge, float x, float y, float z){
    Cube cube(forge.shaderProgram);
    cube.Translate(glm::vec3(x, y, z));
    forge.cubes.push_back(cube);
}

NB_MODULE(concept_forge, m) {
    m.def("add", &add, "a"_a, "b"_a, "Adds two numbers");

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
        .def_rw("cubes", &ConceptForge::cubes)
        .def("set_selected", &ConceptForge::SetSelected);

    // For testing
    m.def("add_cube", &AddCube);
}
