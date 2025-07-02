#include "WindowManager.hpp"

#include <utility>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(entt::registry &registry, int w, int h, std::string name, bool fullscreen) {
    auto constants = registry.ctx().get<Constants>();
    constants.WINDOW_WIDTH = w;
    constants.WINDOW_HEIGHT = h;
    constants.ASPECT_RATIO = static_cast<float>(w) / static_cast<float>(h);
    constants.WINDOW_NAME = std::move(name);
    constants.FULLSCREEN = fullscreen;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if(fullscreen){
        const auto monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Init window
        constants.WINDOW_WIDTH = mode->width;
        constants.WINDOW_HEIGHT = mode->height;
        window = glfwCreateWindow(mode->width, mode->height, constants.WINDOW_NAME.c_str(), monitor, nullptr);
    }
    else{
        window = glfwCreateWindow(constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT, constants.WINDOW_NAME.c_str(), nullptr, nullptr);
    }
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
    }

    // Make context
    glfwMakeContextCurrent(window);

    // GLAD manages function pointers for OpenGL
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Resize support (Not Perfect)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT);
}

// Destructor
Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

auto Window::ScreenClearFlags(glm::vec4 color) -> void {
    // Clear Screen with this color
    glEnable(GL_DEPTH_TEST);
    // Clear Color Buffer and Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, color.a);
}
