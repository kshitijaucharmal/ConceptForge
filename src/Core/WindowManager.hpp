#pragma once

#include "imgui.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

#include <string>

#include <entt/entt.hpp>
#include <Components/Constants.hpp>

class Window {
public:
    GLFWwindow *window;

    Window(entt::registry &registry, int w, int h, std::string name, bool fullscreen=false);
    ~Window();

    static void ScreenClearFlags(entt::registry &registry);
};
