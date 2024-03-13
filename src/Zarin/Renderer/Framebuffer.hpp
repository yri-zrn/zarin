#pragma once

#include "Zarin/Core.hpp"

namespace zrn {

struct FramebufferSpecification {
    uint32_t Width, Height;
    uint32_t Samples = 1;

    bool SwapchainTarget = false;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    // virtual FramebufferSpecification& GetSpecification() = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;
    virtual RendererID GetColorAttachmentRendererID() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
};

} // namespace zrn