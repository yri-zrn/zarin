#include "Renderer.hpp"

#include "Zarin/Platform/OpenGL/OpenGLShader.hpp"

namespace zrn
{

Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

void Renderer::Init() {
    RenderCommand::Init();
}

void Renderer::Shutdown() {
    // Render
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(PerspectiveCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {

}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);

    vertex_array->Bind();
    RenderCommand::DrawIndexed(vertex_array);
}

void Renderer::Draw(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const Ref<Texture2D>& texture) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", mesh->Transform);
    
    texture->Bind();

    mesh->Bind();
    RenderCommand::DrawIndexed(mesh->GetVertexArray());
}

} // namespace zrn
