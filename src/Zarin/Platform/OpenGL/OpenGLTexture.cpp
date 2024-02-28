#include "OpenGLTexture.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <glad/glad.h>

namespace zrn {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_Path(path) {
    
    SDL_Surface* image = IMG_Load(path.c_str());
    // SDL_RWops* rwop = SDL_RWFromFile(tex_path, "rb");
    // SDL_Surface* temp_image = IMG_Load_RW(rwop, 1);
    // SDL_Surface* image = SDL_ConvertSurfaceFormat(temp_image, SDL_PIXELFORMAT_ABGR8888, 0);

    GLenum internal_format = 0, data_format = 0;
    if (image->format->BytesPerPixel == 4) {
        internal_format = GL_RGBA8;
        data_format = GL_RGBA;
    }
    else if (image->format->BytesPerPixel == 3) {
        internal_format = GL_RGB8;
        data_format = GL_RGB;
    }

    ZRN_CORE_ASSERT(internal_format & data_format, "Image format not supported");

    m_Width = image->w;
    m_Height = image->h;
    void* data = image->pixels;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internal_format, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, data_format, GL_UNSIGNED_BYTE, data);

    SDL_FreeSurface(image);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, m_RendererID);
}

} // namespace zrn