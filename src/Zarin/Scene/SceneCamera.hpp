#pragma once

#include "Renderer/Camera.hpp"

namespace zrn {

// TODO: add orthographic camera support 
class SceneCamera : public Camera {
public:
    enum class ProjectionType { Perspective  = 0, Orthographic = 1 };

public:
    SceneCamera() = default;
    SceneCamera(const glm::mat4& projection);
    virtual ~SceneCamera() = default;

    void SetPerspective(float fov, float near_clip, float far_clip);

// TODO: better to add Perspective and Orthographic structs
// TODO: with corresponding methods to make code cleaner

    void SetSize(uint32_t width, uint32_t height);

    ProjectionType GetProjectionType() const { return m_ProjectionType; }
    void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

    float GetPerspectiveNearClip() const { return m_PerspectiveNearClip; }
    void SetPerspectiveNearClip(float clip) { m_PerspectiveNearClip = clip; RecalculateProjection(); }

    float GetPerspectiveFarClip() const { return m_PerspectiveFarClip; }
    void SetPerspectiveFarClip(float clip) { m_PerspectiveFarClip = clip; RecalculateProjection(); }

    float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
    void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }

private:
    void RecalculateProjection();
    // min rotation radius in blender = 1 m
    // zoom value determinies how much vamera will be translated in space
    // camera origin translates with camera but is not affected by zoom value

private:
    ProjectionType m_ProjectionType = ProjectionType::Perspective;
    
    float m_PerspectiveFOV         = glm::radians(45.0f);
    float m_PerspectiveAspectRatio = 16.0f/9.0f;
    float m_PerspectiveNearClip    = 0.1f;
    float m_PerspectiveFarClip     = 1000.0f;

};

} // namespace zrn