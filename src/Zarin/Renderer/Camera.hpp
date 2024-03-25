#pragma once

#include <glm/glm.hpp>

namespace zrn {

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4& projection)
        : m_Projection(projection) { }

    virtual ~Camera() = default;
    
    const glm::mat4& GetProjection() const { return m_Projection; }


    // TODO: add
    // void SetPerspective();
    // void SetOrthographic();
protected:
    glm::mat4 m_Projection     = glm::mat4{ 1.0f };
};

} // namespace zrn