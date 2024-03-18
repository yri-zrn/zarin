#include "OpenGLFramebuffer.hpp"

#include <glad/glad.h>

namespace zrn {

static const uint32_t s_MaxFramebufferSize = 8192;

namespace Utils {

static GLenum TextureTarget(bool multisampled) {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

GLenum DataTypeFromTextureFormat(GLenum format) {
    switch (format) {
    case GL_RGBA8:       return GL_UNSIGNED_BYTE;
    case GL_RGB16F:      return GL_FLOAT;
    case GL_RED_INTEGER: return GL_UNSIGNED_BYTE;
    default: ZRN_CORE_ASSERT(false, "Invalid texture format");
    }
    return 0;
}

static void CreateTextures(bool multisampled, RendererID* outID, uint32_t count) {
    glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void BindTexture(bool multisampled, RendererID ID) {
    glBindTexture(TextureTarget(multisampled), ID);
}

static void AttachColorTexture(RendererID ID, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, DataTypeFromTextureFormat(internal_format), nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), ID, 0);
}

static void AttachDepthTexture(RendererID ID, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    }
    else {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, TextureTarget(multisampled), ID, 0);
}

bool IsDepthFormat(FramebufferTextureFormat format) {
    switch (format) {
    case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
    default: return false;
    }
}

GLenum ZarinFBTextureFormatToGL(FramebufferTextureFormat format) {
    switch (format) {
    case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
    case FramebufferTextureFormat::RGB16F:      return GL_RGB16F;
    case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
    default: ZRN_CORE_ASSERT(false, "Invalid texture format");
    }
    return 0;
}

} // namespace Utils

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
    : m_Specification(specification) {
    
    for (auto spec : m_Specification.Attachments.Attachments) {
        if (!Utils::IsDepthFormat(spec.TextureFormat))
            m_ColorAttachmentSpecifications.emplace_back(spec);
        else
            m_DepthAttachmentSpecification = spec;
    }

    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
}

void OpenGLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Should you bind attachments here?

    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void OpenGLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::BindAttachments() {
    for (size_t slot = 0; slot < m_ColorAttachments.size(); ++slot)
        glBindTextureUnit(slot, m_ColorAttachments[slot]);
}


void OpenGLFramebuffer::Invalidate() {
    if (m_RendererID) {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);

        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    bool multisample = m_Specification.Samples > 1;

    if (m_ColorAttachmentSpecifications.size()) {
        m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
        Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

        for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
            Utils::BindTexture(multisample, m_ColorAttachments[i]);
            switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
            case FramebufferTextureFormat::RGBA8:
                Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                break;
            case FramebufferTextureFormat::RGB16F:
                Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGB16F, GL_RGB, m_Specification.Width, m_Specification.Height, i);
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
                break;
            }
        }
    }

    if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
        Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
        Utils::BindTexture(multisample, m_DepthAttachment);
        switch (m_DepthAttachmentSpecification.TextureFormat) {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
            break;
        }
    }

    if (m_ColorAttachments.size() > 1) {
        ZRN_CORE_ASSERT(m_ColorAttachments.size() <= 4, "");
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_ColorAttachments.size(), buffers);
    }
    else if (m_ColorAttachments.empty()) {
        glDrawBuffer(GL_NONE);
    }

    ZRN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
        ZRN_CORE_WARN("Attempter to resize framebuffer to {0}, {1}", width, height);
        return;
    }

    m_Specification.Width = width;
    m_Specification.Height = height;
    
    Invalidate();
}

void OpenGLFramebuffer::ClearAttachment(uint32_t attachment_index, int value) {
    ZRN_CORE_ASSERT(attachment_index < m_ColorAttachments.size(), "Invalid attachment index");

    auto& spec = m_ColorAttachmentSpecifications[attachment_index];
    glClearTexImage(m_ColorAttachments[attachment_index], 0, Utils::ZarinFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}

} // namespace zrn
