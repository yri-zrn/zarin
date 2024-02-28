#include "Texture.hpp"

#include "Renderer.hpp"
#include "Zarin/Platform/OpenGL/OpenGLTexture.hpp"

namespace zrn {

Ref<Texture2D> Texture2D::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(path);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace zrn