//
// Created by kshitij on 2/13/26.
//

#include "PythonEditor.hpp"

#include "../../../external/imgui/imgui.h"
#include "Components/Constants.hpp"
#define PY_SSIZE_T_CLEAN
#include <iostream>
#include <TextEditor.h>
#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <Systems/Primitives/PrimitivesSystem.hpp>

#include "Components/Fonts.hpp"
#include "glm/gtx/string_cast.hpp"
namespace py = pybind11;

struct Pos {
    float x=0, y=0, z=0;
    Pos(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
};

namespace PythonEditor
{
    static bool python_initialized = false;
    static std::string python_error = "";
    static std::string console_output = "";
    static py::scoped_interpreter* guard = nullptr;
    static py::dict* registry = nullptr;
    TextEditor editor;
    std::string code = "add_cube(Pos(1,2,3))";

    std::string execute_and_capture(const std::string& user_code) {
        auto globals = py::globals();

        // The 'Harness' script
        std::string wrapper = R"(
import sys, io
sys.stdout = io.StringIO()
try:
    exec(user_code)
    captured_output = sys.stdout.getvalue()
finally:
    sys.stdout = sys.__stdout__ # Always reset!
)";

        // Pass the user's code into the Python environment as a variable
        globals["user_code"] = user_code;

        // Run the harness
        py::exec(wrapper, globals);

        // Pull the result back out
        return globals["captured_output"].cast<std::string>();
    }

    void Init()
    {
        if (!python_initialized) {
            guard = new py::scoped_interpreter{};
            python_initialized = true;

            registry = new py::dict();
            (*registry)["entities"] = py::list();

            py::globals()["Pos"] = py::class_<Pos>(py::module_::import("__main__"), "Pos")
                .def(py::init<float, float, float>())
                .def_readwrite("x", &Pos::x)
                .def_readwrite("y", &Pos::y)
                .def_readwrite("z", &Pos::z);

            py::globals()["Transform"] = py::class_<Transform>(py::module_::import("__main__"), "Transform")
                .def(py::init<>())
                .def_readwrite("name", &Transform::name)
                .def_readwrite("position", &Transform::position)
                .def_readwrite("rotation", &Transform::rotation)
                .def_readwrite("eulerAngles", &Transform::eulerAngles)
                .def_readwrite("scale", &Transform::scale)
                .def_readwrite("model", &Transform::model)
                .def_readwrite("parent", &Transform::parent)
                .def_readwrite("first_child", &Transform::first_child)
                .def_readwrite("prev_sibling", &Transform::prev_sibling)
                .def_readwrite("next_sibling", &Transform::next_sibling)
                .def("__repr__", [](const Transform& t) {
                    return py::str("Transform(name='" + t.name + "', pos=" +
                                  glm::to_string(t.position) + ")");
                });
        }
        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Python());
        editor.SetPalette(TextEditor::GetCatppuccinMochaPalette());
        editor.SetText(code);
    }

    void Destroy()
    {
        delete registry;
        delete guard;
        python_initialized = false;
    }
    void Show(entt::registry& registry)
    {
        const auto constants = registry.ctx().get<Constants>();
        const float w = constants.SCENE_WIDTH + constants.SCENE_X;
        const float h = constants.SCENE_HEIGHT;
        ImGui::SetNextWindowPos(ImVec2(0, constants.SCENE_HEIGHT), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(w-400, h), ImGuiCond_Appearing);
        ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::PushFont(registry.ctx().get<Fonts>().FontsDict["CodeFont"]);
        editor.Render("TextEditor", ImVec2(w, h-300));
        ImGui::PopFont();

        if (ImGui::Button("Run",ImVec2(w/4, 20)))
        {
            console_output.clear();
            python_error.clear();  // Clear previous error

            try {
                PyErr_Clear();
                console_output = execute_and_capture(editor.GetText().c_str());
            } catch (py::error_already_set& ex) {
                // Get the actual Python error message
                python_error = ex.what();

                // Also extract traceback
                if (PyErr_Occurred()) {
                    PyObject *ptype, *pvalue, *ptraceback;
                    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

                    if (pvalue) {
                        python_error = std::string(PyUnicode_AsUTF8(PyObject_Str(pvalue)));
                    }
                }
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(w-400, constants.SCENE_HEIGHT), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(400, constants.WINDOW_HEIGHT - h), ImGuiCond_Appearing);
        ImGui::Begin("Console", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


        ImGui::TextColored(ImVec4(1,1,1,1), "%s", console_output.c_str());
        if (!python_error.empty()) {
            ImGui::TextColored(ImVec4(1,0.5f,0.5f,1), "Error: %s", python_error.c_str());
            ImGui::TextColored(ImVec4(1,0.5f,0.5f,1), "Execution failed");
        }

        ImGui::End();

    }
}