#include "Camera.hpp"

Camera::Camera()
    : m_position(glm::vec3(0.0f, 0.0f, 0.0f)), 

    m_pitch(0.0f),
    m_yaw(0.0f),
    m_roll(0.0f),

    m_fov(45.0f), 
    m_nearClip(0.1f), 
    m_farClip(100.0f), 

    m_firstMouse(true), 
    m_lastX(0.0f), 
    m_lastY(0.0f),
    m_sensitivity(0.005f) {
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
    //Keep angles in bounds
    m_yaw = remainder(m_yaw, 2*M_PI);
    m_pitch = remainder(m_pitch, 2*M_PI);
    m_roll = remainder(m_roll, 2*M_PI);
    //Calculate rotation matrix
    glm::mat4 rotationMatrix = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
    //Apply rotations
    m_front = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
    m_up = glm::vec3(rotationMatrix * glm::vec4(0.0f,1.0f, 0.0f, 1.0f));
    m_right = glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Camera::processKeyboardInput(int deltaTime) {
    float velocity = deltaTime * 5 * 10e-10;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
        m_position += m_front * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
        m_position -= m_front * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
        m_position -= m_right * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
        m_position += m_right * velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
        m_roll -= velocity;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
        m_roll += velocity;
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

    m_yaw += xVelocity;
    m_pitch += yVelocity;

    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateVectors();
}