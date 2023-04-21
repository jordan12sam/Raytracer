#include "Window.hpp"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window* windowObject = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowObject->onFramebufferSize(window, width, height);
}

Window::Window(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::isOpen() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    return !glfwWindowShouldClose(m_window);
}

void Window::onFramebufferSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}