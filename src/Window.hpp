#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    bool isOpen();

    virtual void onFramebufferSize(GLFWwindow* window, int width, int height);

    inline GLFWwindow* getWindow(){return m_window;}

private:
    int m_width;
    int m_height;
    const char* m_title;
    GLFWwindow* m_window;
};