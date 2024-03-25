#include "Framebuffer.hpp"

#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace zrn {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return CreateRef<OpenGLFramebuffer>(specification);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace zrn