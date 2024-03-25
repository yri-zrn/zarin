#pragma once

#include "Renderer/RendererAPI.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Material.hpp"
#include "Renderer/Camera.hpp"

#include "Scene/Components.hpp"
#include "Scene/SceneCamera.hpp"
#include "Scene/ViewportCamera.hpp"

namespace zrn {

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const glm::mat4& transform, Camera& camera);
    static void BeginScene(ViewportCamera& camera);
    static void EndScene();

    // static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform = glm::mat4(1.0f));
    // TODO: move to Renderer3D
    // static void DrawQuad(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array);

    static void DrawMesh(const glm::mat4& transform, const Ref<Mesh>& mesh, const Ref<Shader>& shader, int entity);
    static void DrawMesh(const glm::mat4& transform, const MeshRendererComponent& mesh_renderer, int entity);

    static void DrawScreen(const Ref<Shader> shader);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static Scope<SceneData> s_SceneData;
};

} // namespace zrn
