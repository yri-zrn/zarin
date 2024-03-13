#pragma once

#include "Zarin/Renderer/RendererAPI.hpp"

namespace zrn {

class OpenGLRendererAPI : public RendererAPI {
public:
    OpenGLRendererAPI() = default;

    virtual void Init() override;

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertex_array) override;

private:

};

} // namespace zrn