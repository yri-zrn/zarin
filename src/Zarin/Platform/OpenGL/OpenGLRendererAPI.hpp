#pragma once

#include "Zarin/Renderer/RendererAPI.hpp"

namespace zrn {

class OpenGLRendererAPI : public RendererAPI {
public:
    OpenGLRendererAPI() = default;

    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) override;

private:

};

} // namespace zrn