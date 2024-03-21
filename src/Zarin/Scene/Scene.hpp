#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Core/Timestep.hpp"
#include "Zarin/Scene/Components.hpp"

#include "Zarin/Renderer/CameraController.hpp"
#include "Zarin/Renderer/Framebuffer.hpp"
#include "Zarin/Renderer/VertexArray.hpp"
#include "Zarin/Renderer/Shader.hpp"

// #include "Entity.hpp"

#include <entt.hpp>

namespace zrn {

// Forward declaration
class Entity;
class ScriptableEntity;

class Scene {
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = "");

    // const Ref<Framebuffer> GetOutputBuffer() const { return m_OutputBuffer; }
    const RendererID GetOutputImageRendererID() const { return m_OutputBuffer->GetColorAttachmentRendererID(); }
    
    void OnBegin();
    void OnViewportResize(uint32_t width, uint32_t height);
    void OnUpdate(Timestep ts);

friend Entity;
friend ScriptableEntity;
private:
    entt::registry m_Registry;

    PerspectiveCameraController m_CameraController;

    Ref<Framebuffer> m_OutputBuffer;

    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

    // zrn::Ref<zrn::Framebuffer> m_GBuffer;
    // zrn::Ref<zrn::VertexArray> m_BufferQuad;

    // TODO: add
    // SceneRenderer m_SceneRenderer;
    // Viewport m_Viewport;

};

} // namespace zrn
