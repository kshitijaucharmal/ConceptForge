//
// Created by kshitij on 2/14/26.
//

#ifndef CONCEPTFORGE_FONTS_HPP
#define CONCEPTFORGE_FONTS_HPP
#include <unordered_map>
#include <string>

#include "imgui.h"

struct Fonts
{
    // Initialized in GuiSystem.cpp
    std::unordered_map<std::string, ImFont*> FontsDict;
};

#endif //CONCEPTFORGE_FONTS_HPP