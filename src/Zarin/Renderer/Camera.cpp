#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace zrn {

PerspectiveCamera::PerspectiveCamera(float FOV_deg, float aspect_ratio, float near_plane, float far_plane)
    : m_FOV(FOV_deg), m_ViewMatrix(1.0f), m_NearPlane(near_plane), m_FarPlane(far_plane)
    , m_ProjectionMatrix( glm::perspective(glm::radians(FOV_deg), aspect_ratio, near_plane, far_plane) ) {
    
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateViewMatrix() {
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

} // namespace zrn
