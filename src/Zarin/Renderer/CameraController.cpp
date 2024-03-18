#include "CameraController.hpp"

#include "Zarin/Input/Input.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace zrn {

PerspectiveCameraController::PerspectiveCameraController(float FOV_deg, float aspect_ratio, float near_plane, float far_plane)
    : m_AspectRatio(aspect_ratio)
    , m_Camera(FOV_deg, aspect_ratio, near_plane, far_plane) {
    glm::vec3 new_orientation = {
        std::cos(glm::radians(m_Pitch)) * std::cos(glm::radians(m_Yaw)),
        std::sin(glm::radians(m_Pitch)),
        std::cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw))
    };

    m_Camera.SetOrientation(new_orientation);
    
    m_Camera.SetPosition(m_CameraPosition);
}

void PerspectiveCameraController::OnUpdate(Timestep ts) {
    if (zrn::Input::IsKeyPressed(zrn::KeyCode::w)) {
        m_CameraPosition += Speed * m_Camera.GetOrientation() * (float)ts;
    }
    else if (zrn::Input::IsKeyPressed(zrn::KeyCode::s)) {
        m_CameraPosition -= Speed * m_Camera.GetOrientation() * (float)ts;
    }
    if (zrn::Input::IsKeyPressed(zrn::KeyCode::a)) {
        m_CameraPosition -= Speed * glm::normalize(glm::cross(m_Camera.GetOrientation(), m_Camera.GetUp())) * (float)ts;
    }
    else if (zrn::Input::IsKeyPressed(zrn::KeyCode::d)) {
        m_CameraPosition += Speed * glm::normalize(glm::cross(m_Camera.GetOrientation(), m_Camera.GetUp())) * (float)ts;
    }
    if (zrn::Input::IsKeyPressed(zrn::KeyCode::q)) {
        m_CameraPosition -= Speed * m_Camera.GetUp() * (float)ts;
    }
    else if (zrn::Input::IsKeyPressed(zrn::KeyCode::e)) {
        m_CameraPosition += Speed * m_Camera.GetUp() * (float)ts;
    }

    if (m_OrbitCamera) {

        glm::vec2 offset = zrn::Input::GetMousePosition() - m_OldMousePosition;
        // ZRN_CORE_INFO("{0}; {1}", offset.x, offset.y);

        m_OldMousePosition = zrn::Input::GetMousePosition();

        m_Yaw += offset.x * Sensitivity * (float)ts;
        m_Pitch -= offset.y * Sensitivity * (float)ts;

        if (m_Pitch > 89.0f) m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        // when A and S pressed orientation does not work well (idk why)

        glm::vec3 new_orientation = {
            std::cos(glm::radians(m_Pitch)) * std::cos(glm::radians(m_Yaw)),
            std::sin(glm::radians(m_Pitch)),
            std::cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw))
        };

        m_Camera.SetOrientation(new_orientation);
    }

    m_Camera.SetPosition(m_CameraPosition);
}

void PerspectiveCameraController::OnEvent(zrn::Event& event) {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<KeyPressedEvent>(ZRN_BIND_EVENT_FN(PerspectiveCameraController::OnKeyPressedEvent));
    dispatcher.Dispatch<KeyReleasedEvent>(ZRN_BIND_EVENT_FN(PerspectiveCameraController::OnKeyReleasedEvent));

    dispatcher.Dispatch<MouseButtonPressedEvent>(ZRN_BIND_EVENT_FN(PerspectiveCameraController::OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(ZRN_BIND_EVENT_FN(PerspectiveCameraController::OnMouseButtonReleasedEvent));
}

void PerspectiveCameraController::OnResize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    m_Camera.SetProjectionMatrix( glm::perspective(glm::radians(m_Camera.GetFOV()), m_AspectRatio, m_Camera.GetNearPlane(), m_Camera.GetFarPlane()) );
}

bool PerspectiveCameraController::OnKeyPressedEvent(KeyPressedEvent& event) {
    if (event.GetKeyCode() == KeyCode::LeftShift) {
        Speed = 20.0f;
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnKeyReleasedEvent(KeyReleasedEvent& event) {
    if (event.GetKeyCode() == KeyCode::LeftShift) {
        Speed = 10.0f;
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) {
    if (event.GetMouseButton() == zrn::MouseButton::Middle) {
        m_OrbitCamera = true;
        m_OldMousePosition = Input::GetMousePosition();
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) {
    if (event.GetMouseButton() == MouseButton::Middle) {
        m_OrbitCamera = false;
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& event) {
    OnResize(event.GetWidth(), event.GetHeight());
    return false;
}


} // namespace zrn
