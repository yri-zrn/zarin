#include "SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Zarin/Renderer/Camera.hpp"

namespace zrn {

SceneCamera::SceneCamera(const glm::mat4& projection)
    : Camera(projection) {
    RecalculateProjection();
}

void SceneCamera::SetPerspective(float fov, float near_clip, float far_clip) {

    if (m_ProjectionType == ProjectionType::Perspective) {
        m_PerspectiveFOV      = fov;
        m_PerspectiveNearClip = near_clip;
        m_PerspectiveFarClip  = far_clip;
    }
    else if (m_ProjectionType == ProjectionType::Orthographic) {

    }

    RecalculateProjection();
}

void SceneCamera::SetSize(uint32_t width, uint32_t height) {
    m_PerspectiveAspectRatio = (float)width / (float)height;
    
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {
    m_Projection = glm::perspective(m_PerspectiveFOV, m_PerspectiveAspectRatio, m_PerspectiveNearClip, m_PerspectiveFarClip);
}

} // namespace zrn
