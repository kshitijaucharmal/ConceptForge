//
// Created by kshitij on 2/25/26.
//

#include "BaseBindings.hpp"

#include "Components/Primitives/Transform.hpp"
#include "entt/entt.hpp"

namespace BaseBindings
{
    void BindRegistry(py::module_& m, entt::registry& registry)
    {
        // Bind entity type
        py::class_<entt::entity>(m, "Entity")
            .def("__repr__", [](entt::entity e)
            {
                return std::to_string(static_cast<uint32_t>(e));
            });

        m.def("print_all", [&registry]() {
            for (const auto view = registry.view<Transform>(); auto entity : view) {
                auto& t = view.get<Transform>(entity);
                py::print("Entity:", static_cast<uint32_t>(entity), "Name:", t.name);
            }
        });

        // Bind registry with explicit per-component methods
        py::class_<entt::registry>(m, "Registry")
            .def(py::init<>())

            .def("create", [](entt::registry& reg) {
                return reg.create();
            })
            .def("destroy", [](entt::registry& reg, entt::entity e) {
                reg.destroy(e);
            })
            .def("valid", &entt::registry::valid);
    }
}
