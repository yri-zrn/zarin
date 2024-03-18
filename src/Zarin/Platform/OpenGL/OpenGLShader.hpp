#pragma once

#include "Zarin/Renderer/Shader.hpp"

#include <glad/glad.h>

#include <unordered_map>

namespace zrn {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const std::string& GetName() const override { return m_Name; }
    virtual const std::string& GetFilepath() const override { return m_Filepath; }
    
    virtual void Load(const std::string& filepath) override;
    virtual void Reload() override;

    virtual void SetInt(const std::string& name, int value) override;
    virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
    virtual void SetFloat(const std::string& name, float value) override;
    virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
    virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
    virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
    virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

private:
    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    RendererID Compile(std::unordered_map<GLenum, std::string> shader_sources);

private:
    uint32_t m_RendererID;
    std::string m_Name;

    // For reload
    std::string m_Filepath;
};

} // namespace zrn
