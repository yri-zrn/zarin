#include "Zarin/Core.hpp"
#include "Buffer.hpp"

#include "Renderer.hpp"
#include "Zarin/Platform/OpenGL/OpenGLBuffer.hpp"

namespace zrn {

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return new OpenGLVertexBuffer(vertices, size);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return new OpenGLIndexBuffer(indices, size);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace zrn