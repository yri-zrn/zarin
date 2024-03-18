#include "OpenGLShader.hpp"
#include "Zarin/Core.hpp"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace zrn {

namespace Utils {

static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex")    return GL_VERTEX_SHADER;
    if (type == "fragment")  return GL_FRAGMENT_SHADER;

    ZRN_CORE_ASSERT(false, "Unknown shader type '{0}'", type);
    return 0;
}

} // namespace Utils

OpenGLShader::OpenGLShader(const std::string& filepath)
    : m_Filepath(filepath) {
    Load(m_Filepath);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string &vertex_source, const std::string &fragment_source) 
    : m_Name(name) {

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertex_source;
    sources[GL_FRAGMENT_SHADER] = fragment_source;

    m_RendererID = Compile(sources);
    // auto renderer_ID = Compile(sources);
    // ZRN_CORE_ASSERT(renderer_ID, "Failed to load shader");
    // m_RendererID = renderer_ID;
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Load(const std::string& filepath) {
    std::string source = ReadFile(filepath);
    auto shader_sources = PreProcess(source);
    auto renderer_ID = Compile(shader_sources);
    if (!renderer_ID) {
        ZRN_CORE_ERROR("Failed to load shader");
        return;
    }
    m_RendererID = renderer_ID;

    // Extract name from filepath
    auto last_slash = filepath.find_last_of("/\\");
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
    auto last_dot = filepath.rfind(".");
    auto count = last_dot == std::string::npos ? filepath.size() - last_slash : last_dot - last_slash;

    m_Name = filepath.substr(last_slash, count);
}

void OpenGLShader::Reload() {
    Load(m_Filepath);
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
        shader_sources[Utils::ShaderTypeFromString(type)] =
            source.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? source.size() - 1 : next_line_pos));
    }

    return shader_sources;
}

RendererID OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shader_sources) {
    
    GLuint program = glCreateProgram();

    ZRN_CORE_ASSERT(shader_sources.size() <= 8, "zarin supports only 8 shaders for now");
    std::array<GLenum, 8> shader_IDs;
    int shader_ID_index = 0;

    for (auto&&[shader_type, shader_source] : shader_sources) {
        GLuint shader = glCreateShader(shader_type);

        const GLchar* source = shader_source.c_str();
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);

        GLint is_compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if(is_compiled == GL_FALSE) {
            GLint max_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

            std::vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);
            
            glDeleteShader(shader);

            ZRN_CORE_ERROR((char*)info_log.data());
            ZRN_CORE_ERROR("Shader compilation failure");
            // ZRN_CORE_ASSERT(false, "Shader compilation failure");
            return 0;
        }

        glAttachShader(program, shader);
        shader_IDs[shader_ID_index++] = shader;
    }

    glLinkProgram(program);

    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);
        
        glDeleteProgram(program);
        for (auto id : shader_IDs)
            glDeleteShader(id);

        ZRN_CORE_ERROR((char*)info_log.data());
        ZRN_CORE_ERROR("Shader linking failure");
        // ZRN_CORE_ASSERT(false, "Shader linking failure");
        return 0;
    }

    for (auto id : shader_IDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }

    return program;
}

void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value) {
    UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
    UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
    UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
    UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
    UploadUniformMat3(name, matrix);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
    UploadUniformMat4(name, matrix);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
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