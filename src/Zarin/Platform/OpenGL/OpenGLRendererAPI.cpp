#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

namespace zrn
{

void OpenGLRendererAPI::Init() {
    glEnable(GL_DEPTH_TEST);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array) {
    vertex_array->Bind();
    
    glDrawElements(
        GL_TRIANGLES,
        vertex_array->GetIndexBuffer()->GetCount(),
        GL_UNSIGNED_INT,
        nullptr
    );
}

} // namespace zrn
