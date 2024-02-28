#include "OpenGLShader.hpp"
#include "Zarin/Core.hpp"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace zrn {

static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex")    return GL_VERTEX_SHADER;
    if (type == "fragment")  return GL_FRAGMENT_SHADER;

    ZRN_CORE_ASSERT(false, "Unknown shader type '{0}'", type);
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
    std::string source = ReadFile(filepath);
    auto shader_sources = PreProcess(source);
    Compile(shader_sources);
}

OpenGLShader::OpenGLShader(const std::string &vertex_source, const std::string &fragment_source) {
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertex_source;
    sources[GL_FRAGMENT_SHADER] = fragment_source;

    Compile(sources);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    std::string result;

    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();

    }
    else {
        ZRN_CORE_ERROR("Could not open file '{0}'", filepath);
    }
    
    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
    std::unordered_map<GLenum, std::string> shader_sources;

    const char* type_token = "#type";
    size_t type_token_length = strlen(type_token);
    size_t pos = source.find(type_token, 0);

    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        ZRN_CORE_ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + type_token_length + 1;
        std::string type = source.substr(begin, eol - begin);
        ZRN_CORE_ASSERT(type == "vertex" || type == "fragment", "Invalid shader type specifier");

        size_t next_line_pos = source.find_first_not_of("\r\n", eol);
        pos = source.find(type_token, next_line_pos);
        shader_sources[ShaderTypeFromString(type)] =
            source.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? source.size() - 1 : next_line_pos));
    }

    return shader_sources;
}

void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shader_sources) {
    
    GLuint program = glCreateProgram();
    std::vector<GLenum> glShaderIDs(shader_sources.size());

    for (auto&&[shader_type, shader_source] : shader_sources) {
        GLuint shader = glCreateShader(shader_type);

        const GLchar* source = (const GLchar *)shader_source.c_str();
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            
            glDeleteShader(shader);

            ZRN_CORE_ERROR((char*)infoLog.data());
            ZRN_CORE_ASSERT(false, "Shader compilation failure");
            return;
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs.push_back(shader);
    }

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteProgram(program);
        for (auto id : glShaderIDs)
            glDeleteShader(id);

        ZRN_CORE_ERROR((char*)infoLog.data());
        ZRN_CORE_ASSERT(false, "Shader linking failure");
        return;
    }

    for (auto id : glShaderIDs)
        glDetachShader(program, id);

    m_RendererID = program;
}

void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}


void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace zrn