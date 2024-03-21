#pragma once

#include "Zarin/Renderer/Camera.hpp"

namespace zrn {

class SceneCamera : public Camera {
public:
    SceneCamera() = default;
    SceneCamera(const glm::mat4& projection);
    virtual ~SceneCamera() = default;

    void SetPerspective(float fov, float near_clip, float far_clip);
    
    void SetSize(uint32_t width, uint32_t height);

private:
    void RecalculateProjection();
    // min rotation radius in blender = 1 m
    // zoom value determinies how much vamera will be translated in space
    // camera origin translates with camera but is not affected by zoom value

private:
    float m_FOV         = 45.0f;
    float m_AspectRatio = 16.0f/9.0f;
    float m_NearClip    = 0.1f;
    float m_FarClip     = 1000.0f;

};

} // namespace zrn