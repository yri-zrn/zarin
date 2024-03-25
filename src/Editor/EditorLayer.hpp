#pragma once

#include "Core/Core.hpp"
#include "Core/Timestep.hpp"
#include "Core/Layer.hpp"
#include "Events/Event.hpp"
#include "Scene/Scene.hpp"
#include "Editor/Widgets/SceneHierarchyPanel.hpp"

#include <glm/glm.hpp>

namespace zrn {

class EditorLayer : public Layer {
public:
    EditorLayer() = default;
    virtual ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Timestep ts) override;
    virtual void OnEvent(Event& event) override;
    virtual void OnImGuiRender() override;

private:
    bool OnKeyPressed(KeyPressedEvent& event);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

private:

    glm::vec2 m_ViewportBounds[2];
    glm::vec2 m_ViewportMousePosition;
    uint32_t m_ViewportWidth;
    uint32_t m_ViewportHeight;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    int m_GizmoType = -1;
    
    Ref<Scene> m_ActiveScene;
    SceneHierarchyPanel m_SceneHierarchyPanel;


    ShaderLibrary m_ShaderLibrary;

    glm::vec2 m_ViewportSize;
    glm::vec2 m_InitialViewportSize;
};

} // namespace zrn