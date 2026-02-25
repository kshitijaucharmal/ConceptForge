//
// Created by kshitij on 2/13/26.
//

#ifndef CONCEPTFORGE_PYTHONEDITOR_HPP
#define CONCEPTFORGE_PYTHONEDITOR_HPP

#include "entt/entt.hpp"
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace py = pybind11;
namespace PythonEditor
{
    void Init();
    void Destroy();
    void Show(entt::registry &registry);
}


#endif //CONCEPTFORGE_PYTHONEDITOR_HPP