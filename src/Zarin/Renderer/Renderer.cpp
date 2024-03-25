#include "Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"


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

void Renderer::BeginScene(const glm::mat4& transform, Camera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
}

void Renderer::BeginScene(ViewportCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
}

void Renderer::EndScene() {

}

// void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform) {
//     shader->Bind();
//     shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
//     shader->SetMat4("u_Transform", transform);

//     vertex_array->Bind();
//     RenderCommand::DrawIndexed(vertex_array);
// }

// void Renderer::DrawQuad(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array) {
//     shader->Bind();

//     vertex_array->Bind();

//     RenderCommand::Draw(vertex_array);
// }

void Renderer::DrawMesh(const glm::mat4& transform, const Ref<Mesh>& mesh, const Ref<Shader>& shader, int entity) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);

    mesh->Bind();
    RenderCommand::DrawIndexed(mesh->GetVertexArray());
}

void Renderer::DrawMesh(const glm::mat4& transform, const MeshRendererComponent& mesh_renderer, int entity) {
    const auto& shader = mesh_renderer.Material->GetShader();
    const auto& mesh = mesh_renderer.Mesh;

    mesh_renderer.Material->GetAlbedo()->Bind();
    Renderer::DrawMesh(transform, mesh, shader, entity);
}

void Renderer::DrawScreen(const Ref<Shader> shader) {

}

} // namespace zrn
