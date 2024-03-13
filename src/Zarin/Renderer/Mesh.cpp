#include "Mesh.hpp"

namespace zrn {

Mesh::Mesh(std::vector<float>& vertex_data, std::vector<uint32_t>& indices, const BufferLayout& layout)
    : m_VertexData(vertex_data), m_Indices(indices) {

    m_VertexArray = zrn::VertexArray::Create();
    m_VertexArray->Bind();

    zrn::Ref<zrn::VertexBuffer> vertex_buffer;
    vertex_buffer = zrn::VertexBuffer::Create(m_VertexData.data(), m_VertexData.size() * sizeof(float));
    vertex_buffer->SetLayout(layout);

    zrn::Ref<zrn::IndexBuffer> index_buffer;
    index_buffer = zrn::IndexBuffer::Create(m_Indices.data(), m_Indices.size());

    m_VertexArray->AddVertexBuffer(vertex_buffer);
    m_VertexArray->SetIndexBuffer(index_buffer);
}

void Mesh::Bind() const {
    m_VertexArray->Bind();
}

void Mesh::Unbind() const {
    m_VertexArray->Unbind();
}

} // namespace zrn
