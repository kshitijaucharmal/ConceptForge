// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <memory>

#include "setup.hpp"
using namespace Engine;

namespace nb = nanobind;

float add(float a, float b){
    return a + b;
}

std::unique_ptr<ConceptForge> forge;
void Init(){
    forge = std::make_unique<ConceptForge>();
    std::cout << "Initialised" << std::endl;
}

bool WindowShouldClose(){
    return forge->WindowShouldClose();
}

void CalcDeltaTime(){
    forge->DeltaTimeCalc();
}
void ProcessInput(){
    forge->ProcessInput();
}
void Render(){
    forge->Render();
}
void CalcProjection(){
    forge->CalcProjection();
}
void GUIManagement(){
    forge->GUIManagement();
}

NB_MODULE(concept_forge, m) {
    m.def("add", &add);
    m.def("init", &Init);
    m.def("window_should_close", &WindowShouldClose);
    m.def("calc_delta_time", &CalcDeltaTime);
    m.def("process_input", &ProcessInput);
    m.def("render", &Render);
    m.def("calc_projection", &CalcProjection);
    m.def("gui_management", &GUIManagement);
    m.doc() = "Class";
}
