#pragma once

#include "Zarin/Renderer/Framebuffer.hpp"

namespace zrn {

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& specification);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    virtual RendererID GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

    void Invalidate();

    virtual void Resize(uint32_t width, uint32_t height) override;

private:
    RendererID m_RendererID = 0;
    RendererID m_ColorAttachment = 0, m_DepthAttachment = 0;
    FramebufferSpecification m_Specification;
};

} // namespace zrn