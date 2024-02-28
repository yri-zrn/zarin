#pragma once

// #include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// #include <glm/gtc/type_ptr.hpp>

namespace zrn
{

class PerspectiveCamera {
public:
    PerspectiveCamera(float FOV_deg, float aspect, float near_plane, float far_plane);
    
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }

    const glm::vec3& GetOrientation() const { return m_Orientation; }
    void SetOrientation(const glm::vec3& orientation) {
        m_Orientation = orientation;
        RecalculateViewMatrix();
    }

    const glm::vec3& GetUp() const { return m_Up; }
    void SetUp(const glm::vec3& up) {
        m_Up = up;
        RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    
    float Speed       = 10.0f;
    float Sensitivity = 50.0f;

private:
    void RecalculateViewMatrix();

private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up          = glm::vec3(0.0f, 1.0f, 0.0f);
};

} // namespace zrn
