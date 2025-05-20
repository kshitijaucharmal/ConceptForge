#include <nanobind/nanobind.h>

int add(int a, int b) { return a + b; }

NB_MODULE(concept_forge, m) {
  m.def("add", &add);

  m.doc() = "Forge your concept with ConceptForge";
}
