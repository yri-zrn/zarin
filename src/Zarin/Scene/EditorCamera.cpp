#include "EditorCamera.hpp"

namespace zrn {

EditorCamera::EditorCamera() {
    UpdateProjection();
    UpdatePosition();
    UpdateView();
}

glm::quat EditorCamera::GetOrientation() const {
    return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}

glm::vec3 EditorCamera::GetForwardDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 EditorCamera::GetRightDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::GetUpDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}


void EditorCamera::UpdatePosition() {
    m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
}

void EditorCamera::UpdateProjection() {
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    m_ViewProjection = m_Projection * m_View;
}

void EditorCamera::UpdateView() {
    glm::quat orientation = GetOrientation();
    m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_View = glm::inverse(m_View);

    m_ViewProjection = m_Projection * m_View;
}

} // namespace zrn