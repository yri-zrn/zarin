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
        FramebufferTextureFormat::RED_INTEGER,    // entity ID
        FramebufferTextureFormat::DEPTH24STENCIL8 // depth
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

void Scene::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity);
}

Entity Scene::GetEntityOnScreen(int x, int y) {
    int entity_id = m_OutputBuffer->ReadPixel(1, x, y);
    return entity_id == -1 ? Entity{ } : Entity{ (entt::entity)entity_id, this };
}

Entity Scene::GetPrimaryCameraEntity() {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        const auto& cc = view.get<CameraComponent>(entity);
        if (cc.Primary)
            return { entity, this };
    }
    return {};
}

void Scene::OnBegin() {
    m_CameraController = CreateRef<EditorCameraController>(&m_Camera);
}

void Scene::OnEvent(Event& event) {
    m_CameraController->OnEvent(event);
    
    // EventDispatcher dispatcher{ event };
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    m_CameraController->OnViewportResize(m_ViewportWidth, m_ViewportHeight);
    m_OutputBuffer->Resize(m_ViewportWidth, m_ViewportHeight);

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        auto& camera = view.get<CameraComponent>(entity);

        if (!camera.FixedAspectRatio)
            camera.Camera.SetSize(width, height);
    }
}

void Scene::OnUpdateEditor(Timestep ts) {
    m_CameraController->OnUpdate(ts);

    m_OutputBuffer->Bind();
    Renderer::BeginScene(m_Camera);
    {
        zrn::RenderCommand::SetClearColor({ 0.8f, 0.8f, 0.8f, 1.0f });
        zrn::RenderCommand::Clear();

        m_OutputBuffer->ClearAttachment(1, -1);
        
        auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
        for (auto entity : group) {
            auto[transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
            const auto& shader = mesh.Material->GetShader();
            shader->SetInt("u_Albedo", 0);
            Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
        }
    }
    Renderer::EndScene();
    m_OutputBuffer->Unbind();
}

void Scene::OnUpdateRuntime(Timestep ts) {
    // {
    //     m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
    //         if (!nsc.Instance) {
    //             // nsc.Construct(entity);
    //             nsc.InstantiateScript(&nsc);
    //             nsc.Instance->m_Entity = { entity, this };
    //             // if (nsc.OnCreate)
    //                 //  nsc.OnCreate();
    //             if (nsc.OnCreate)
    //                 nsc.OnCreate(nsc.Instance);
    //         }
    //         if (nsc.OnUpdate)
    //             nsc.OnUpdate(nsc.Instance, ts);
    //     });
    // }

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

    // if (main_camera) {
    //     auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
    //     m_OutputBuffer->Bind();
    //     Renderer::BeginScene(main_camera_transform, *main_camera);
    //     {
    //         zrn::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
    //         zrn::RenderCommand::Clear();
    //         for (auto entity : group) {
    //             auto[transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
    //             const auto& shader = mesh.Material->GetShader();
    //             // shader->SetFloat3("u_Color", glm::vec3(1.0f, 0.5f, 0.8f));
    //             shader->SetInt("u_Albedo", 0);
    //             Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
    //         }
    //     }
    //     Renderer::EndScene();
    //     m_OutputBuffer->Unbind();
    // }
}

template<typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
    ZRN_CORE_ASSERT(false, "");
}

template<>
void Scene::OnComponentAdded(Entity entity, TagComponent& component) {
    
}

template<>
void Scene::OnComponentAdded(Entity entity, TransformComponent& component) {
    
}

template<>
void Scene::OnComponentAdded(Entity entity, CameraComponent& component) {
    OnViewportResize(m_ViewportWidth, m_ViewportHeight);
}

template<>
void Scene::OnComponentAdded(Entity entity, MeshRendererComponent& component) {
}

template<>
void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component) {

}

} // namespace zrn