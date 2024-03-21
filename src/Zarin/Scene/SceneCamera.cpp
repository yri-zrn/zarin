#include "SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace zrn {

SceneCamera::SceneCamera(const glm::mat4& projection)
    : Camera(projection) {
    RecalculateProjection();
}

void SceneCamera::SetPerspective(float fov, float near_clip, float far_clip) {
    m_FOV      = fov;
    m_NearClip = near_clip;
    m_FarClip  = far_clip;

    RecalculateProjection();
}

void SceneCamera::SetSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {
    m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
}

} // namespace zrn
