#pragma once

#include "RendererAPI.hpp"
#include "RenderCommand.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "Camera.hpp"

#include "Zarin/Scene/Components.hpp"
#include "Zarin/Scene/SceneCamera.hpp"
#include "Zarin/Scene/EditorCamera.hpp"

namespace zrn {

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const glm::mat4& transform, Camera& camera);
    static void BeginScene(EditorCamera& camera);
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
