#pragma once

#include "VertexArray.hpp"
#include "RendererAPI.hpp"

namespace zrn {

class RenderCommand {
public:
    inline static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    inline static void Clear() {
        s_RendererAPI->Clear();
    }

    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) {
        s_RendererAPI->DrawIndexed(vertex_array);
    }

private:
    static RendererAPI* s_RendererAPI;
};

} // namespace zrn