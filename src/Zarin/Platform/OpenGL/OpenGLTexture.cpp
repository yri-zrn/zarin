#include "OpenGLTexture.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



namespace zrn {

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) 
    : m_Width(width), m_Height(height) {

    m_InternalFormat = GL_RGBA8;
    m_Format = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
    : m_Path(filepath) {
    
    SDL_Surface* image = IMG_Load(filepath.c_str());
    ZRN_CORE_ASSERT(image, "Failed to load image");

    // maybe will be used in the future ?
    // SDL_RWops* rwop = SDL_RWFromFile(filepath.c_str(), "rb");
    // SDL_Surface* temp_image = IMG_Load_RW(rwop, 1);
    // SDL_Surface* image = SDL_ConvertSurfaceFormat(temp_image, SDL_PIXELFORMAT_ABGR8888, 0);

    if (image->format->BytesPerPixel == 4) {
        m_InternalFormat = GL_RGBA8;
        m_Format = GL_RGBA;
    }
    else if (image->format->BytesPerPixel == 3) {
        m_InternalFormat = GL_RGB8;
        m_Format = GL_RGB;
    }

    ZRN_CORE_ASSERT(m_InternalFormat & m_Format, "Image format not supported");

    m_Width = image->w;
    m_Height = image->h;
    void* data = image->pixels;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);

    SDL_FreeSurface(image);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    uint32_t bytes_per_pixel = m_Format == GL_RGBA ? 4 : 3;
    ZRN_CORE_ASSERT(size == m_Width * m_Height * bytes_per_pixel, "Data must be entire texture");

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
}


void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, m_RendererID);
}

} // namespace zrn