#include "SceneHierarchyPanel.hpp"

#include <imgui.h>

#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace zrn {

namespace Utils {
    // vid src: Transform Component UI | Game Engine series
    // static void DrawVec3Control(const std::string& label, glm::vec3& value, float reset_value = 0.0f, float column_width = 100.0f);
} // namespace Utils

SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene) {
    SetContext(scene);
}

void SceneHierarchyPanel::SetContext(Scene* context) {
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRender() {
    ImGui::Begin("Scene");
    {
        m_Context->m_Registry.view<entt::entity>().each([&](auto entity_handle) {
            Entity entity{ entity_handle, m_Context };
            DrawEntityNode(entity);
        });
    }

    if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
        m_SelectionContext = {};
    }

    if (ImGui::BeginPopupContextWindow("CreateEntity", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
        if (ImGui::MenuItem("Create Entity"))
            m_Context->CreateEntity();

        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");
    {
        if (m_SelectionContext) {
            DrawComponents(m_SelectionContext);
            if (ImGui::Button("Add component")) {
                ImGui::OpenPopup("AddComponent");
            }

            if (ImGui::BeginPopup("AddComponent")) {
                if (ImGui::MenuItem("Camera")) {
                    m_SelectionContext.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Mesh renderer")) {
                    m_SelectionContext.AddComponent<MeshRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (m_SelectionContext == entity) flags |= ImGuiTreeNodeFlags_Selected;
    
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity), flags, "%s", tag.c_str());
    if (ImGui::IsItemClicked()) {
        // TODO: add selection changed callback
        m_SelectionContext = entity;
    }

    bool entity_deleted = false;
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete entity"))
            entity_deleted = true;
        ImGui::EndPopup();
    }

    if (opened) {
        ImGui::TreePop();
    }

    if (entity_deleted) {
        m_Context->DestroyEntity(entity);
        if (m_SelectionContext == entity)
            m_SelectionContext = {};
    }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
    // TODO:add function for drawing components
    // TODO:template<typename T, Func> DrawComponent<T>(std::string name, Func func);

    if (entity.HasComponent<TagComponent>()) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        static char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());

        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }
    }

    const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

    if (entity.HasComponent<TransformComponent>()) {
        bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), tree_node_flags, "%s", "Transform");

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });
        ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
        if (ImGui::Button("...", ImVec2{ 20.0f, 20.0f }))
            ImGui::OpenPopup("ComponentSettings");
        ImGui::PopStyleVar();
        
        bool remove_component = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove component")) {
                remove_component = true;
            }
            ImGui::EndPopup();
        }

        if (open) {
            auto& tc = entity.GetComponent<TransformComponent>();

            ImGui::DragFloat3("Translation", glm::value_ptr(tc.Translation), 0.1f);
            auto rotation = glm::degrees(tc.Rotation);
            ImGui::DragFloat3("Rotation",    glm::value_ptr(rotation),       1.0f);
            tc.Rotation = glm::radians(rotation);
            ImGui::DragFloat3("Scale",       glm::value_ptr(tc.Scale),       0.1f);

            ImGui::TreePop();
        }

        if (remove_component)
            entity.RemoveComponent<TransformComponent>();
    }

    if (entity.HasComponent<CameraComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), tree_node_flags, "%s", "Camera")) {
            auto& cc = entity.GetComponent<CameraComponent>();

            ImGui::Checkbox("Primary camera", &cc.Primary);

            static const char* projection_type_string[] = { "Perspective", "Orthographic" };
            static const char* current_projection_type_string = projection_type_string[(int)cc.Camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", current_projection_type_string)) {
                
                for (int i = 0; i < 2; ++i) {
                    bool is_selected = current_projection_type_string == projection_type_string[i];
                    if (ImGui::Selectable(projection_type_string[i], is_selected)) {
                        current_projection_type_string = projection_type_string[i];
                        cc.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (cc.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
                float fov       = cc.Camera.GetPerspectiveFOV();
                float near_clip = cc.Camera.GetPerspectiveNearClip();
                float far_clip  = cc.Camera.GetPerspectiveFarClip();

                if (ImGui::DragFloat("Focal length", &fov, 0.1f, 1.0f, 250.0f))
                    cc.Camera.SetPerspectiveFOV(fov);

                if (ImGui::DragFloat("Near clip", &near_clip, 0.1f, 0.0001f, 10000.0f))
                    cc.Camera.SetPerspectiveNearClip(near_clip);

                if (ImGui::DragFloat("Far clip", &far_clip, 0.1f, 0.0001f, 10000.0f))
                    cc.Camera.SetPerspectiveFarClip(far_clip);
            }

            if (cc.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {

            }

            ImGui::TreePop();
        }
    }

    if (entity.HasComponent<MeshRendererComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(MeshRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "%s", "Mesh Renderer")) {
            auto& tc = entity.GetComponent<MeshRendererComponent>();

            ImGui::TextUnformatted("Mesh albedo");
            ImVec2 image_size = { 256, 256 };
            auto albedo_ID = tc.Material->GetAlbedo()->GetRendererID();
            ImGui::Image(reinterpret_cast<void*>(albedo_ID), image_size, ImVec2(0, 1), ImVec2(1, 0));




            
            ImGui::TreePop();
        }
    }
}

} // namespace zrn
