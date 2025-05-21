// Nano Bind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "setup.hpp"

namespace nb = nanobind;

float add(float a, float b){
    return a + b;
}

void Init(){
    std::cout << "Hello World" << std::endl;
}

NB_MODULE(concept_forge, m) {
    m.def("add", &add);
    m.def("init", &Init);
    m.doc() = "Class";
}
