#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position), 
    m_worldUp(up), 
    m_yaw(yaw), 
    m_pitch(pitch), 
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_fov(45.0f), 
    m_nearClip(0.1f), 
    m_farClip(100.0f), 
    m_firstMouse(true), 
    m_lastX(0.0f), 
    m_lastY(0.0f),
    m_sensitivity(0.2f) {
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::getPosition() {
    return m_position;
}

void Camera::updateVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processKeyboardInput(int deltaTime) {
    float velocity = deltaTime * 10e-9;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
        m_position += m_front * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
        m_position -= m_front * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
        m_position -= m_right * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
        m_position += m_right * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_position += m_up * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_position -= m_up * velocity;
    // TODO: Make program close safely
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwTerminate();

    //std::cout << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;
}

void Camera::processMouseInput(float xoffset, float yoffset, bool constrainPitch) {

    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    float xVelocity = m_lastX - xoffset;
    float yVelocity = m_lastY - yoffset;

    m_lastX = xoffset;
    m_lastY = yoffset;

    m_yaw -= xVelocity;
    m_pitch += yVelocity;

    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateVectors();
}