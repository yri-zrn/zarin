#pragma once

#include "Scene/Components.hpp"
#include "Scene/ViewportCamera.hpp"
#include "Scene/ViewportCameraController.hpp"

#include "Core/Core.hpp"
#include "Core/Timestep.hpp"

#include "Renderer/Framebuffer.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"

// #include "Entity.hpp"

#include <entt.hpp>

namespace zrn {

// Forward declaration
class Entity;
class ScriptableEntity;
class SceneHierarchyPanel;

class Scene {
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = "Unnamed");
    void DestroyEntity(Entity entity);

    const Ref<Framebuffer> GetOutputBuffer() const { return m_OutputBuffer; }
    const RendererID GetOutputImageRendererID() const { return m_OutputBuffer->GetColorAttachmentRendererID(); }
    Entity GetEntityOnScreen(int x, int y);
    
    Entity GetPrimaryCameraEntity();
    const ViewportCamera& GetViewportCamera() const { return m_Camera; }

    void OnBegin();
    void OnEvent(Event& event);
    void OnUpdateRuntime(Timestep ts);
    void OnUpdateEditor(Timestep ts);

    void OnViewportResize(uint32_t width, uint32_t height);
    
friend Entity;
friend ScriptableEntity;
friend SceneHierarchyPanel;
private:
    template<typename T>
    void OnComponentAdded(Entity entity, T& component);

private:
    entt::registry m_Registry;

    ViewportCamera m_Camera;
    Ref<ViewportCameraController> m_CameraController;

    Ref<Framebuffer> m_OutputBuffer;

    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;

    // zrn::Ref<zrn::Framebuffer> m_GBuffer;
    // zrn::Ref<zrn::VertexArray> m_BufferQuad;

    // TODO: add
    // SceneRenderer m_SceneRenderer;
    // Viewport m_Viewport;
};

} // namespace zrn
