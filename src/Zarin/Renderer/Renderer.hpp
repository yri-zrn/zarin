#pragma once

#include "RendererAPI.hpp"
#include "RenderCommand.hpp"
#include "VertexArray.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace zrn {

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(PerspectiveCamera& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform = glm::mat4(1.0f));

    // TODO: move to Renderer3D
    static void Draw(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const Ref<Texture2D>& texture);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static Scope<SceneData> s_SceneData;
};

} // namespace zrn
