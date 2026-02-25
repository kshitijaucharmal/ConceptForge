//
// Created by kshitij on 2/25/26.
//
#pragma once

#include <string>
#include <pybind11/embed.h>

namespace py = pybind11;
namespace PythonRunner {
    void Init();
    void BindRegistry(const py::module_& m);
    void Destroy();
    std::string Execute(const std::string& user_code);
}
