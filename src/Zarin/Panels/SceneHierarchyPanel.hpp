#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Scene/Scene.hpp"
#include "Zarin/Scene/Entity.hpp"

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
