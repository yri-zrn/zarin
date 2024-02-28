#pragma once

#include "RendererAPI.hpp"
#include "RenderCommand.hpp"
#include "VertexArray.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

namespace zrn
{

class Renderer {
public:
    static void Init();

    static void BeginScene(PerspectiveCamera& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform = glm::mat4(1.0f));

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;
};

} // namespace zrn
