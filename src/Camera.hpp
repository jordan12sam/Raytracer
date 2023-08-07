#pragma once

#define _USE_MATH_DEFINES

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>

class Camera {
    public:
        Camera();
        glm::mat4 getViewMatrix();
        void processKeyboardInput(int deltaTime);
        void processMouseInput(float xoffset, float yoffset, bool constrainPitch = true);
        glm::vec3 getPosition();

    private:
        void updateVectors();

        glm::vec3 m_position;

        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;

        float m_yaw;
        float m_pitch;
        float m_roll;

        float m_fov;
        float m_nearClip;
        float m_farClip;

        bool m_firstMouse;
        float m_lastX;
        float m_lastY;
        float m_sensitivity;
};
