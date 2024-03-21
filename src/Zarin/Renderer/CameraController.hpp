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
    PerspectiveCameraController(float FOV_deg = 45.0f, float aspect_ratio = 16.0f/9.0f, float near_plane = 0.1f, float far_plane = 1000.0f);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    void OnResize(uint32_t width, uint32_t height);

    PerspectiveCamera& GetCamera() { return m_Camera; }
    const PerspectiveCamera& GetCamera() const { return m_Camera; }

public:
    float Speed       = 5.0f;
    float Sensitivity = 30.0f;

private:
    bool OnKeyPressedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
    bool OnWindowResized(WindowResizeEvent& event);

private:
    float m_Pitch = -45.0f, m_Yaw = -45.0f;

    float m_AspectRatio;
    PerspectiveCamera m_Camera;
    glm::vec3 m_CameraPosition = { -8.0f, 10.0f, 8.0f };
    bool m_OrbitCamera = false;
    glm::vec2 m_OldMousePosition;
};

} // namespace zrn
