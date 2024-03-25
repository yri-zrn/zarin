#pragma once

#include "Core/Core.hpp"

namespace zrn {

enum class FramebufferTextureFormat {
    None = 0,
    RGBA8,
    RGB16F,
    RED_INTEGER,

    DEPTH24STENCIL8,

    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : TextureFormat(format) { }
    
    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    // TODO: Add filtering / wrap
};

struct FramebufferAttachmentSpecification {
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : Attachments(attachments) { }
    
    std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification {
    uint32_t Width, Height;
    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool SwapchainTarget = false;
};

// should contain clear color?
class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void BindAttachments() = 0;

    // virtual FramebufferSpecification& GetSpecification() = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;
    virtual RendererID GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
    virtual uint32_t GetColorAttachmentCount() const = 0;
    virtual RendererID GetDepthAttachmentRendererID() const = 0;

    virtual int ReadPixel(uint32_t attachment_index, int x, int y) = 0;
    
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual void ClearAttachment(uint32_t attachment_index, int value) = 0;
};

} // namespace zrn