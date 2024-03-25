#pragma once

#include "Zarin/Renderer/Framebuffer.hpp"

namespace zrn {

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& specification);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void BindAttachments() override;

    virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    virtual RendererID GetColorAttachmentRendererID(uint32_t index = 0) const override {
        ZRN_CORE_ASSERT(index < m_ColorAttachments.size(), "Wrong color attachment index");
        return m_ColorAttachments[index];
    }
    virtual uint32_t GetColorAttachmentCount() const override { return m_ColorAttachments.size(); }
    virtual RendererID GetDepthAttachmentRendererID() const override {
        return m_DepthAttachment;
    }

    void Invalidate();

    virtual int ReadPixel(uint32_t attachment_index, int x, int y) override;

    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual void ClearAttachment(uint32_t attachment_index, int value) override;

private:
    RendererID m_RendererID = 0;
    FramebufferSpecification m_Specification;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<RendererID> m_ColorAttachments;
    RendererID m_DepthAttachment = 0;
};

} // namespace zrn