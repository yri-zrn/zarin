#pragma once

#include "Renderer/VertexArray.hpp"

namespace zrn {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;
    
    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
        return m_VertexBuffers;
    }
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const override {
        return m_IndexBuffer;
    }

private:
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;

    uint32_t m_RendererID;
};

} // namespace zrn