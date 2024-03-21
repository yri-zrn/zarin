#pragma once

#include <glm/glm.hpp>

namespace zrn {

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4& projection)
        : m_Projection(projection) { }

    virtual ~Camera() = default;
    
    const glm::mat4& GetProjection() const { return m_Projection; }

    // TODO: add
    // void SetPerspective();
    // void SetOrthographic();
protected:
    glm::mat4 m_Projection = glm::mat4(1.0f);
};

class PerspectiveCamera {
public:
    PerspectiveCamera(float FOV_deg, float aspect_ratio, float near_plane, float far_plane);
    
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) {
        m_Position = position; RecalculateViewMatrix();
    }

    const glm::vec3& GetOrientation() const { return m_Orientation; }
    void SetOrientation(const glm::vec3& orientation) {
        m_Orientation = orientation; RecalculateViewMatrix();
    }

    const glm::vec3& GetUp() const { return m_Up; }
    void SetUp(const glm::vec3& up) {
        m_Up = up; RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    void SetProjectionMatrix(const glm::mat4& matrix) {
        m_ProjectionMatrix = matrix; RecalculateViewMatrix();
    }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    
    float GetFOV() const { return m_FOV; }
    float GetNearPlane() const { return m_NearPlane; }
    float GetFarPlane() const { return m_FarPlane; }
    
private:
    void RecalculateViewMatrix();

private:
    glm::vec3 m_Position    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up          = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_ProjectionMatrix, m_ViewMatrix, m_ViewProjectionMatrix;

    float m_FOV;
    float m_NearPlane, m_FarPlane;

};

} // namespace zrn
