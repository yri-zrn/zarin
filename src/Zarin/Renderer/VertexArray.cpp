#include "VertexArray.hpp"

#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace zrn {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexArray>();

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace zrn