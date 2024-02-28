#include "Shader.hpp"

#include "Zarin/Renderer/Renderer.hpp"
#include "Zarin/Platform/OpenGL/OpenGLShader.hpp"

namespace zrn {

Ref<Shader> Shader::Create(const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(filepath);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string& vertex_source, const std::string& fragment_source) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(vertex_source, fragment_source);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}


} // namespace zrn