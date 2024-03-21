#include "Scene.hpp"
#include "Zarin/Renderer/Renderer.hpp"

#include "Entity.hpp"
#include "ScriptableEntity.hpp"

#include <glm/glm.hpp>

namespace zrn {

Scene::Scene() {
    FramebufferSpecification framebuffer_spec;
    framebuffer_spec.Width = 1280;
    framebuffer_spec.Height = 720;
    framebuffer_spec.Samples = 1;
    framebuffer_spec.Attachments = { 
        FramebufferTextureFormat::RGBA8,          // output image
        FramebufferTextureFormat::DEPTH24STENCIL8 // output image
    };
    m_OutputBuffer = Framebuffer::Create(framebuffer_spec);
}

Scene::~Scene() {

}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity = { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>(name);
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
}

void Scene::OnBegin() {

}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    m_OutputBuffer->Resize(m_ViewportWidth, m_ViewportHeight);

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        auto& camera = view.get<CameraComponent>(entity);

        if (!camera.FixedAspectRatio)
            camera.Camera.SetSize(width, height);
    }
}

void Scene::OnUpdate(Timestep ts) {
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            if (!nsc.Instance) {
                // nsc.Construct(entity);

                nsc.InstantiateScript(&nsc);
                nsc.Instance->m_Entity = { entity, this };

                // if (nsc.OnCreate)
                    //  nsc.OnCreate();
                
                if (nsc.OnCreate)
                    nsc.OnCreate(nsc.Instance);
            }

            if (nsc.OnUpdate)
                nsc.OnUpdate(nsc.Instance, ts);
        });
    }

    SceneCamera* main_camera = nullptr;
    glm::mat4 main_camera_transform{ 1.0f };

    {
        auto view = m_Registry.view<CameraComponent, TransformComponent>();
        for (auto entity : view) {
            auto[camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

            if (camera.Primary) {
                main_camera = &camera.Camera;
                main_camera_transform = transform.GetTransform();
                break;
            }
        }
    }

    {
        auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);

        m_OutputBuffer->Bind();
        Renderer::BeginScene(*main_camera, main_camera_transform);
        {
            zrn::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
            zrn::RenderCommand::Clear();

            for (auto entity : group) {
                auto[transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
                auto shader = mesh.ShaderAsset;
                auto albedo = mesh.AlbedoTexture;
                shader->Bind();
                albedo->Bind();
                // shader->SetFloat3("u_Color", glm::vec3(1.0f, 0.5f, 0.8f));
                shader->SetInt("u_Albedo", 0);
                Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
            }
        }
        Renderer::EndScene();
        m_OutputBuffer->Unbind();
    }
}

} // namespace zrn