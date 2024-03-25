#include "Texture.hpp"

#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace zrn {

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(width, height);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(path);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace zrn