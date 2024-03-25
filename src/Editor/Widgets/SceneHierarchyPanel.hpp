#pragma once

#include "Core/Core.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"

namespace zrn {

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(Scene* scene);

    void SetContext(Scene* context);

    void SelectEntity(const Entity entity) { m_SelectionContext = entity; }
    Entity GetSelectedEntity() const { return m_SelectionContext; }

    void OnImGuiRender();

friend Scene;
private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);

private:
    Scene* m_Context;
    Entity m_SelectionContext;
};

} // namespace zrn
