#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Renderer/Camera.hpp"
#include "Zarin/Core/Timestep.hpp"

#include "Zarin/Events/ApplicationEvent.hpp"
#include "Zarin/Events/MouseEvent.hpp"
#include "Zarin/Events/KeyEvent.hpp"

namespace zrn {

class PerspectiveCameraController {
public:
    PerspectiveCameraController(float FOV_deg, float aspect_ratio, float near_plane, float far_plane);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    void OnResize(uint32_t width, uint32_t height);

    PerspectiveCamera& GetCamera() { return m_Camera; }
    const PerspectiveCamera& GetCamera() const { return m_Camera; }

public:
    float Speed       = 5.0f;
    float Sensitivity = 18.0f;

private:
    bool OnKeyPressedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
    bool OnWindowResized(WindowResizeEvent& event);

private:
    float m_Pitch = 0.0f, m_Yaw = -90.0f;

    float m_AspectRatio;
    PerspectiveCamera m_Camera;
    glm::vec3 m_CameraPosition;
    bool m_OrbitCamera = false;
    glm::vec2 m_OldMousePosition;
};

} // namespace zrn