#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace zrn
{

PerspectiveCamera::PerspectiveCamera(float FOV_deg, float aspect, float near_plane, float far_plane)
    : m_ProjectionMatrix( glm::perspective(glm::radians(FOV_deg), aspect, near_plane, far_plane) ),
      m_ViewMatrix(1.0f) {
    
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateViewMatrix() {
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

} // namespace zrn
