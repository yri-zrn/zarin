#pragma once

#include "RendererAPI.hpp"
#include "RenderCommand.hpp"
#include "VertexArray.hpp"

namespace zrn
{

class Renderer {
public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertex_array);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

} // namespace zrn
