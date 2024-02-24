#include "OpenGLContext.hpp"

namespace zrn {

OpenGLContext::OpenGLContext(SDL_Window* window)
: m_WindowHandle(window) { }

void OpenGLContext::Init() {
    m_GLContext = SDL_GL_CreateContext(m_WindowHandle);
    ZRN_CORE_ASSERT(m_GLContext, "Failed to create SDL OpenGL context");
    SDL_GL_MakeCurrent(m_WindowHandle, m_GLContext);
    
    int VSync_status = SDL_GL_SetSwapInterval( 1 );
    ZRN_CORE_ASSERT(!VSync_status, "Failed to set VSync");

    int openGL_status = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
    ZRN_CORE_ASSERT(openGL_status, "Failed to create OpenGL context");

    ZRN_CORE_INFO("OpenGL Info: ");
    ZRN_CORE_INFO("  Vendor   : {0}", (char*)glGetString(GL_VENDOR));
    ZRN_CORE_INFO("  Renderer : {0}", (char*)glGetString(GL_RENDERER));
    ZRN_CORE_INFO("  Version  : {0}", (char*)glGetString(GL_VERSION));

}

void OpenGLContext::Destroy() {
    SDL_GL_DeleteContext(m_GLContext);
}

void OpenGLContext::SwapBuffers() {
    SDL_GL_SwapWindow(m_WindowHandle);
}

} // namespace zrn