#include "EditorCameraController.hpp"
#include "Zarin/Input/Input.hpp"

namespace zrn {

EditorCameraController::EditorCameraController(EditorCamera* camera)
    : m_Camera(camera) {
    
    UpdatePanSpeed();
    UpdateZoomSpeed();

    m_RotationSpeed = 0.8f;
}

void EditorCameraController::OnUpdate(Timestep ts) {
    const auto& mouse_position = Input::GetMousePosition();
    auto delta = mouse_position - m_InitialMousePosition;
    m_InitialMousePosition = mouse_position;
    delta *= ts;

    if (glm::length(delta) > 0.00001f) {
        if (Input::IsMouseButtonPressed(MouseButton::Middle)) {
            if (Input::IsKeyPressed(KeyCode::LeftShift))
                MousePan(delta);
            else if (Input::IsKeyPressed(KeyCode::LeftControl))
                MouseZoom(-delta.y);
            else
                MouseRotate(delta);
        }
    }
}

void EditorCameraController::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    UpdatePanSpeed();

    m_Camera->m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
    m_Camera->UpdateProjection();
}

void EditorCameraController::OnEvent(Event& event) {
    EventDispatcher dispatcher{ event };

    dispatcher.Dispatch<MouseScrolledEvent>(ZRN_BIND_EVENT_FN(EditorCameraController::OnMouseScroll));
}

bool EditorCameraController::OnMouseScroll(MouseScrolledEvent& event) {
    float delta = event.GetYOffset() * 0.05f;
    MouseZoom(delta);
    return false;
}


void EditorCameraController::MousePan(const glm::vec2& delta) {
    m_Camera->m_FocalPoint += -m_Camera->GetRightDirection() * delta.x * m_PanSpeed.x * m_Camera->m_Distance;
    m_Camera->m_FocalPoint += m_Camera->GetUpDirection() * delta.y * m_PanSpeed.y * m_Camera->m_Distance;

    m_Camera->UpdatePosition();
    m_Camera->UpdateView();
}

void EditorCameraController::MouseRotate(const glm::vec2& delta) {
    float yaw_sign = m_Camera->GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_Camera->m_Yaw += yaw_sign * delta.x * m_RotationSpeed;
    m_Camera->m_Pitch += delta.y * m_RotationSpeed;
    
    m_Camera->UpdatePosition();
    m_Camera->UpdateView();
}

void EditorCameraController::MouseZoom(float delta) {
    auto& distance = m_Camera->m_Distance;
    distance -= delta * m_ZoomSpeed;
    // if (distance < 1.0f) {
    //     m_Camera->m_FocalPoint += m_Camera->GetForwardDirection();
    //     distance = 1.0f;
    // }
    UpdateZoomSpeed();
    m_Camera->UpdatePosition();
    m_Camera->UpdateView();
}

void EditorCameraController::UpdatePanSpeed() {
    const float max_pan_speed = 2.4f;

    float x = std::min(m_ViewportWidth / 1000.0f, max_pan_speed);
    float y = std::min(m_ViewportHeight / 1000.0f, max_pan_speed);

    m_PanSpeed = {
        0.0366f * x * x - 0.1778f * x + 0.3021f,
        0.0366f * y * y - 0.1778f * y + 0.3021f
    };
}

void EditorCameraController::UpdateZoomSpeed() {
    float distance = m_Camera->m_Distance * 0.2f;
    distance = std::max(m_Camera->m_Distance, 0.0f);
    float speed = distance * distance;
    const float max_zoom_speed = 100.0f;
    m_ZoomSpeed = std::min(speed, max_zoom_speed);
}

} // namespace zrn