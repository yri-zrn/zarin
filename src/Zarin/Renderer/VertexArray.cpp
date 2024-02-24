#include "VertexArray.hpp"

#include "Renderer.hpp"
#include "Zarin/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace zrn {

VertexArray* VertexArray::Create() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return new OpenGLVertexArray();

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace zrn