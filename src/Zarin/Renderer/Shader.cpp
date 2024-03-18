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

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(name, vertex_source, fragment_source);

    case RendererAPI::API::None:
        ZRN_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        return nullptr;
    }

    ZRN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

/*************************************************************
*                       Shader Library
**************************************************************/

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
    ZRN_CORE_ASSERT(!Exists(name), "Shader already exists");
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
    Add(shader->GetName(), shader);
}

void ShaderLibrary::ReloadAll() {
    for (auto&&[name, shader] : m_Shaders) {
        shader->Reload();
    }
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) {
    ZRN_CORE_ASSERT(Exists(name), "Shader not found");
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) {
    return m_Shaders.find(name) != m_Shaders.end();
}



} // namespace zrn