#include "Renderer.hpp"

#include "Zarin/Platform/OpenGL/OpenGLShader.hpp"

namespace zrn
{

void Renderer::Init() {
    RenderCommand::Init();
}

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

void Renderer::BeginScene(PerspectiveCamera& camera) {
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {

}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform) {
    shader->Bind();

    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

    vertex_array->Bind();
    RenderCommand::DrawIndexed(vertex_array);
}

void Renderer::Draw(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const Ref<Texture2D>& texture) {
    shader->Bind();
    texture->Bind();

    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", mesh->Transform);

    mesh->GetVertexArray()->Bind();
    RenderCommand::DrawIndexed(mesh->GetVertexArray());
}

} // namespace zrn
