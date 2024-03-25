#pragma once

#include "Core/Timestep.hpp"
#include "Core/Core.hpp"
#include "Renderer/Camera.hpp"
#include "Events/Event.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace zrn {

class ViewportCamera : public Camera {
public:
    ViewportCamera();

    const glm::vec3& GetPosition() const { return m_Position; }
    glm::quat GetOrientation() const;
    glm::vec3 GetForwardDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetUpDirection() const;

    const glm::mat4& GetView() const { return m_View; }
    const glm::mat4& GetViewProjection() const { return m_ViewProjection; }

friend class ViewportCameraController;
private:
    void UpdatePosition();
    void UpdateProjection();
    void UpdateView();

private:
    float m_FOV         = 45.0f;
    float m_AspectRatio = 16.0f / 9.0f;
    float m_NearClip    = 0.01f; 
    float m_FarClip     = 1000.0f;

    float m_Pitch    = 0.0f;
    float m_Yaw      = 0.0f;

    float m_Distance = 10.0f;
    glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_Position   = { 0.0f, 0.0f, 0.0f };

    glm::mat4 m_View;
    glm::mat4 m_ViewProjection;
};

} // namespace zrn