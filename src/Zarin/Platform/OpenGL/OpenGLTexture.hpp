#pragma once

#include "Zarin/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace zrn {

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();
    
    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }

    virtual uint32_t GetRendererID() const override { return m_RendererID; }

    virtual void SetData(void* data, uint32_t size) override;

    virtual void Bind(uint32_t slot = 0) const override;

private:
    uint32_t m_Width;
    uint32_t m_Height;

    std::string m_Path;
    uint32_t m_RendererID;

    GLenum m_InternalFormat;
    GLenum m_Format;
};

} // namespace zrn