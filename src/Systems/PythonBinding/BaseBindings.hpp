//
// Created by kshitij on 2/25/26.
//

#ifndef CONCEPTFORGE_BASEBINDINGS_HPP
#define CONCEPTFORGE_BASEBINDINGS_HPP

#include <pybind11/embed.h>

#include "entt/entt.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

namespace BaseBindings
{

    void BindRegistry(py::module_& m, entt::registry& registry);
}

#endif //CONCEPTFORGE_BASEBINDINGS_HPP