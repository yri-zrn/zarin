#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

namespace zrn
{

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) {
    glDrawElements(
        GL_TRIANGLES,
        vertex_array->GetIndexBuffer()->GetCount(),
        GL_UNSIGNED_INT,
        nullptr
    );
}

} // namespace zrn
