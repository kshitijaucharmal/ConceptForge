//
// Created by kshitij on 2/13/26.
//

#include "PythonEditor.hpp"

#include "../../../external/imgui/imgui.h"
#include "Components/Constants.hpp"
#define PY_SSIZE_T_CLEAN
#include <TextEditor.h>
#include <Systems/Primitives/PrimitivesSystem.hpp>

#include "Components/Fonts.hpp"
#include "glm/gtx/string_cast.hpp"
#include "../../Systems/PythonRunner.hpp"

namespace PythonEditor
{
    static std::string python_error;
    static std::string console_output;
    TextEditor editor;
    std::string code = "import concept_forge as cf\n\n";

    void Init() {
        PythonRunner::Init();

        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Python());
        editor.SetPalette(TextEditor::GetCatppuccinMochaPalette());
        editor.SetText(code);
    }

    void Destroy()
    {
        PythonRunner::Destroy();
    }
    void Show(entt::registry& registry)
    {
        const auto constants = registry.ctx().get<Constants>();
        const float w = constants.SCENE_WIDTH + constants.SCENE_X;
        const float h = constants.SCENE_HEIGHT;

        // Editor
        ImGui::SetNextWindowPos(ImVec2(0, constants.SCENE_HEIGHT), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(w-400, h), ImGuiCond_Appearing);
        ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::PushFont(registry.ctx().get<Fonts>().FontsDict["CodeFont"]);
        editor.Render("TextEditor", ImVec2(w, h-300));
        ImGui::PopFont();

        if (ImGui::Button("Run",ImVec2(w/4, 40)))
        {
            console_output.clear();
            python_error.clear();  // Clear previous error

            try {
                PyErr_Clear();
                console_output = PythonRunner::Execute(editor.GetText().c_str());
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

        // Console
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