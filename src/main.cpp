#include "app.cpp"
#include "Systems/PythonBinding/BaseBindings.hpp"

entt::registry* g_registry;
// Bind everything before starting
PYBIND11_EMBEDDED_MODULE(concept_forge, m) {
    BaseBindings::BindRegistry(m, *g_registry);
}

int main(){
    // Initialization
    App app(1600, 900, "ConceptForge");
    g_registry = &app.registry;
    app.MainLoop();
}