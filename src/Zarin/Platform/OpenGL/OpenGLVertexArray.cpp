#include "OpenGLVertexArray.hpp"

#include <glad/glad.h>

namespace zrn {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::None:      break;
    case ShaderDataType::Float:     return GL_FLOAT;
    case ShaderDataType::Float2:    return GL_FLOAT;
    case ShaderDataType::Float3:    return GL_FLOAT;
    case ShaderDataType::Float4:    return GL_FLOAT;
    case ShaderDataType::Mat3:      return GL_FLOAT;
    case ShaderDataType::Mat4:      return GL_FLOAT;
    case ShaderDataType::Int:       return GL_INT;
    case ShaderDataType::Int2:      return GL_INT;
    case ShaderDataType::Int3:      return GL_INT;
    case ShaderDataType::Int4:      return GL_INT;
    case ShaderDataType::Bool:      return GL_BOOL;
    };

    ZRN_CORE_ASSERT(false, "Unknown ShaderDataType");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
    glCreateVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) {
    ZRN_CORE_ASSERT(vertex_buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

    glBindVertexArray(m_RendererID);
    vertex_buffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertex_buffer->GetLayout();
    for (const auto& element : layout) {
        switch (element.Type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void*>(element.Offset));
            ++index;
            break;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool:
            glEnableVertexAttribArray(index);
            glVertexAttribIPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                layout.GetStride(),
                reinterpret_cast<const void*>(element.Offset));
            ++index;
            break;
        }
    }
    m_VertexBuffers.push_back(vertex_buffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
    glBindVertexArray(m_RendererID);
    index_buffer->Bind();

    m_IndexBuffer = index_buffer;
}

} // namespace zrn