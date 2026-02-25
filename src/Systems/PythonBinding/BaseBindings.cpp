//
// Created by kshitij on 2/25/26.
//

#include "BaseBindings.hpp"

#include "Components/SceneRoot.hpp"
#include "Components/Primitives/Transform.hpp"
#include "entt/entt.hpp"
#include "Systems/Primitives/PrimitivesSystem.hpp"

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

        py::class_<glm::vec3>(m, "Vec3")
            .def(py::init<float, float, float>(), py::arg("x") = 0.0f, py::arg("y") = 0.0f, py::arg("z") = 0.0f)

            .def_readwrite("x", &glm::vec3::x)
            .def_readwrite("y", &glm::vec3::y)
            .def_readwrite("z", &glm::vec3::z)

            .def("__repr__", [](const glm::vec3 &v) {
                return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
            });

        m.def("add_cube", [&registry](
            const glm::vec3 pos,
            const glm::vec3 rot=glm::vec3(0),
            const glm::vec3 scale=glm::vec3(1)
            ) -> void {
            const auto transform = Transform {
                .name = "Cube",
                .position = pos,
                .rotation = glm::quat(rot),
                .eulerAngles = rot,
                .scale = scale,
                .parent = registry.ctx().get<SceneRoot>().entity,
            };
            auto &shaders = registry.ctx().get<ShaderStore>().shaders;
            Primitives::Create(registry, Primitives::PrimitiveType::CUBE, transform, shaders["LitShader"]);
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
