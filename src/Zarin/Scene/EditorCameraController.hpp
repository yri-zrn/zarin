#pragma once
#include "EditorCamera.hpp"
#include "Zarin/Events/Event.hpp"
#include "Zarin/Events/MouseEvent.hpp"
#include "Zarin/Events/KeyEvent.hpp"

namespace zrn {

class EditorCameraController {
public:
    EditorCameraController() = default;
    EditorCameraController(EditorCamera* camera);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& event);
    void OnViewportResize(uint32_t width, uint32_t height);

    float GetRotationSpeed() const { return m_RotationSpeed; }
    void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }

    float GetZoomSpeed() const { return m_ZoomSpeed; }
    void SetZoomSpeed(float speed) { m_ZoomSpeed = speed; UpdateZoomSpeed(); }

    glm::vec2 GetPanSpeed() const { return m_PanSpeed; }
    void SetPanSpeed(glm::vec2 speed) { m_PanSpeed = speed; UpdatePanSpeed(); }

private:
    bool OnMouseScroll(MouseScrolledEvent& event);

private:
    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    void UpdateZoomSpeed();
    void UpdatePanSpeed();

private:
    glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

    glm::vec2 m_PanSpeed;
    float m_RotationSpeed;
    float m_ZoomSpeed;

    EditorCamera* m_Camera;
    uint32_t m_ViewportWidth = 1280;
    uint32_t m_ViewportHeight = 720;
};

} // namespace zrn