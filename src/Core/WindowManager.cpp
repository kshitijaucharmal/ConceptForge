#include "WindowManager.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(entt::registry &registry, int w, int h, std::string name, bool fullscreen) {
    auto constants = registry.ctx().get<Constants>();
    constants.WINDOW_WIDTH = w;
    constants.WINDOW_HEIGHT = h;
    constants.ASPECT_RATIO = (float)w / (float)h;
    constants.WINDOW_NAME = name;
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
        window = glfwCreateWindow(mode->width, mode->height, constants.WINDOW_NAME.c_str(), monitor, NULL);
    }
    else{
        window = glfwCreateWindow(constants.WINDOW_WIDTH, constants.WINDOW_HEIGHT, constants.WINDOW_NAME.c_str(), NULL, NULL);
    }
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
    }

    // Make context
    glfwMakeContextCurrent(window);

    // GLAD manages function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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

void Window::ScreenClearFlags(entt::registry &registry){
    auto constants = registry.ctx().get<Constants>();
    // Clear Screen with this color
    glClearColor(constants.CLEAR_COLOR.r,
                    constants.CLEAR_COLOR.g,
                    constants.CLEAR_COLOR.b,
                    constants.CLEAR_COLOR.a);
    // Clear Color Buffer and Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
