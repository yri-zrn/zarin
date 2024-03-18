#pragma once

#include "VertexArray.hpp"
#include "RendererAPI.hpp"

namespace zrn {

class RenderCommand {
public:
    inline static void Init() {
        s_RendererAPI->Init();
    }

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    inline static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    inline static void Clear() {
        s_RendererAPI->Clear();
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertex_array) {
        s_RendererAPI->DrawIndexed(vertex_array);
    }

    inline static void Draw(const Ref<VertexArray>& vertex_array) {
        s_RendererAPI->Draw(vertex_array);
    }

private:
    static RendererAPI* s_RendererAPI;
};

} // namespace zrn