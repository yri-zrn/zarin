#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Renderer/GraphicsContext.hpp"

#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace zrn {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(SDL_Window* window);
    ~OpenGLContext() = default;

    virtual void Init() override;
    virtual void Destroy() override;
    virtual void SwapBuffers() override;

private:
    SDL_Window* m_WindowHandle;
    SDL_GLContext m_GLContext;
};

} // namespace zrn