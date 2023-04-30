#include "Window.hpp"
#include <iostream>

const unsigned short OPENGL_MAJOR_VERSION = 4;
const unsigned short OPENGL_MINOR_VERSION = 6;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window* windowObject = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowObject->onFramebufferSize(window, width, height);
}

Window::Window(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
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
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool Window::isOpen() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    return !glfwWindowShouldClose(m_window);
}

void Window::onFramebufferSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}