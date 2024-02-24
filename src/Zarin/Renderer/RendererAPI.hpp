#pragma once

#include "VertexArray.hpp"

#include <memory>

#include <glm/glm.hpp>

namespace zrn {

class RendererAPI {
public:
    enum class API {
        None    = 0,
        OpenGL  = 1
    };

public:
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) = 0;

    inline static API GetAPI() { return s_RendererAPI; }

private:
    static API s_RendererAPI;
};

} // namespace zrn