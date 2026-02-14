//
// Created by kshitij on 2/13/26.
//

#include "PythonEditor.hpp"

#include "../../../external/imgui/imgui.h"
#include "Components/Constants.hpp"
#define PY_SSIZE_T_CLEAN
#include <iostream>
#include <Python.h>
#include <TextEditor.h>

namespace PythonEditor
{
    wchar_t *program = nullptr;
    TextEditor editor;

    void Init()
    {
        program = Py_DecodeLocale("ConceptForge", nullptr);
        if (program == nullptr) {
            std::cout << "Could not create program";
            return;
        }


        Py_Initialize();
        static char buffer[8192] = "from time import time,ctime\nprint('Today is', ctime(time()))\n";

        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Python());
        editor.SetText(buffer);
    }

    void Destroy()
    {
        if (Py_FinalizeEx() < 0) {
            exit(120);
        }
        PyMem_RawFree(program);
    }
    void Show(entt::registry& registry)
    {
        const auto constants = registry.ctx().get<Constants>();
        const float w = constants.WINDOW_WIDTH - (constants.SCENE_WIDTH + constants.SCENE_X);
        const float h = constants.SCENE_HEIGHT;
        ImGui::SetNextWindowPos(ImVec2(0, constants.SCENE_HEIGHT), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Appearing);
        ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        editor.Render("TextEditor", ImVec2(-1, h-300));

        if (ImGui::Button("Run",ImVec2(w, 40)))
        {
            PyRun_SimpleString(editor.GetText().c_str());
        }

        ImGui::End();

    }

}
