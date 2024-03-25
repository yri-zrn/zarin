#include "EditorLayer.hpp"

#include "Core/Application.hpp"
#include "Input/Input.hpp"
#include "Scene/AssetManager.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace zrn {

namespace Utils {

static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
    glm::mat4 LocalMatrix(transform);

    // Normalize the matrix.
    if(glm::epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
        return false;

    if(glm::epsilonNotEqual(LocalMatrix[0][3], static_cast<float>(0), glm::epsilon<float>()) ||
       glm::epsilonNotEqual(LocalMatrix[1][3], static_cast<float>(0), glm::epsilon<float>()) ||
       glm::epsilonNotEqual(LocalMatrix[2][3], static_cast<float>(0), glm::epsilon<float>()))
    {
        // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<float>(0);
        LocalMatrix[3][3] = static_cast<float>(1);
    }

    for(glm::length_t i = 0; i < 4; ++i)
    for(glm::length_t j = 0; j < 4; ++j)
        LocalMatrix[i][j] /= LocalMatrix[3][3];

    // Translation
    translation = glm::vec4(LocalMatrix[3]);
    LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);

    glm::vec3 Row[3], Pdum3;

    // Scale
    for(glm::length_t i = 0; i < 3; ++i)
    for(glm::length_t j = 0; j < 3; ++j)
        Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor and normalize first row.
    scale.x = length(Row[0]);
    scale.y = length(Row[1]);
    scale.z = length(Row[2]);

    Row[0] = glm::detail::scale(Row[0], static_cast<float>(1));
    Row[1] = glm::detail::scale(Row[1], static_cast<float>(1));
    Row[2] = glm::detail::scale(Row[2], static_cast<float>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
    Pdum3 = glm::cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if(glm::dot(Row[0], Pdum3) < 0) {
        for(glm::length_t i = 0; i < 3; i++) {
            scale[i] *= static_cast<float>(-1);
            Row[i] *= static_cast<float>(-1);
        }
    }

    // Rotation
    rotation.y = asin(-Row[0][2]);
    if (cos(rotation.y) != 0) {
        rotation.x = atan2(Row[1][2], Row[2][2]);
        rotation.z = atan2(Row[0][1], Row[0][0]);
    } else {
        rotation.x = atan2(-Row[2][0], Row[1][1]);
        rotation.z = 0;
    }
#ifdef hee
    int i, j, k = 0;
    float root, trace = Row[0].x + Row[1].y + Row[2].z;
    if(trace > static_cast<T>(0))
    {
        root = sqrt(trace + static_cast<T>(1.0));
        Orientation.w = static_cast<T>(0.5) * root;
        root = static_cast<T>(0.5) / root;
        Orientation.x = root * (Row[1].z - Row[2].y);
        Orientation.y = root * (Row[2].x - Row[0].z);
        Orientation.z = root * (Row[0].y - Row[1].x);
    } // End if > 0
    else
    {
        static int Next[3] = {1, 2, 0};
        i = 0;
        if(Row[1].y > Row[0].x) i = 1;
        if(Row[2].z > Row[i][i]) i = 2;
        j = Next[i];
        k = Next[j];

        root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

        Orientation[i] = static_cast<T>(0.5) * root;
        root = static_cast<T>(0.5) / root;
        Orientation[j] = root * (Row[i][j] + Row[j][i]);
        Orientation[k] = root * (Row[i][k] + Row[k][i]);
        Orientation.w = root * (Row[j][k] - Row[k][j]);
    } // End if <= 0
#endif

    return true;
}

} // namespace Utils

#ifdef old
EditorLayer::EditorLayer()
    : Layer("Example") {
    {
        m_BufferQuad = VertexArray::Create();
        std::vector<float> vertices = {
                1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,

                1.0f,  1.0f,  1.0f, 1.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f
        };
        auto vertex_buffer = VertexBuffer::Create(vertices.data(), sizeof(float) * vertices.size());
        vertex_buffer->SetLayout({
            { ShaderDataType::Float2, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        m_BufferQuad->AddVertexBuffer(vertex_buffer);

        m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/Blur.glsl");
        m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/GammaCorrection.glsl");

        m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/Lighting.glsl");
    }

    {
        m_Model = AssetManager::LoadModel("C:/dev/zarinApp/assets/models/9mm.obj");
        m_ModelTexture = Texture2D::Create("C:/dev/zarinApp/assets/textures/9mm/GunGS_Albedo.tga.png");

        auto lighting_shader = m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/FlatColorLighting.glsl");
        lighting_shader->Bind();
        lighting_shader->SetInt("u_Texture", 0);

        auto gbuffer = m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/GBuffer.glsl");
        gbuffer->Bind();
        gbuffer->SetInt("u_Texture", 0);
    }

    {
        m_Sphere = AssetManager::LoadModel("C:/dev/zarinApp/assets/models/sphere.obj");

        m_ColorTexture = Texture2D::Create(1, 1);
        // reversed order because computers use little-endian
        uint32_t solid_color = 0xffe48648;
        m_ColorTexture->SetData(&solid_color, sizeof(solid_color));
    }

    {
        m_Floor = AssetManager::LoadModel("C:/dev/zarinApp/assets/models/floor.obj");
        m_FloorDiffuse = Texture2D::Create("C:/dev/zarinApp/assets/textures/planks/agedplanks1-albedo.png");
    }

    {
        m_LightSourceCube = AssetManager::LoadModel("C:/dev/zarinApp/assets/models/small_cube.obj");
        m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/FlatColor.glsl");
    }
}
#endif

void EditorLayer::OnAttach() {
    #ifdef old
    {
        FramebufferSpecification framebuffer_spec;
        framebuffer_spec.Width = 1280;
        framebuffer_spec.Height = 720;
        framebuffer_spec.Samples = 1;
        framebuffer_spec.Attachments = { 
            FramebufferTextureFormat::RGBA8 // output image
        };
        m_OutputBuffer = Framebuffer::Create(framebuffer_spec);
    }
    {
        FramebufferSpecification framebuffer_spec;
        framebuffer_spec.Width = 1280;
        framebuffer_spec.Height = 720;
        framebuffer_spec.Samples = 1;
        framebuffer_spec.Attachments = { 
            FramebufferTextureFormat::RGB16F, // position
            FramebufferTextureFormat::RGB16F, // normal
            FramebufferTextureFormat::RGBA8,  // albedo
            FramebufferTextureFormat::RGBA8,  // specular
            FramebufferTextureFormat::Depth
        };
        m_GBuffer = Framebuffer::Create(framebuffer_spec);
    }

    #endif

    m_ActiveScene = CreateRef<Scene>();
    m_ActiveScene->OnBegin();

    auto glock_entity = m_ActiveScene->CreateEntity("Glock");
    auto glock_model = AssetManager::LoadModel("C:/dev/zarinApp/assets/models/9mm.obj", (int)glock_entity);
    auto glock_albedo = Texture2D::Create("C:/dev/zarinApp/assets/textures/9mm/GunGS_Albedo.tga.png");
    auto glock_shader = m_ShaderLibrary.Load("C:/dev/zarinApp/assets/shaders/Texture.glsl");
    MeshRendererComponent mesh_renderer;
    mesh_renderer.Mesh = glock_model;
    mesh_renderer.Material = CreateRef<Material>();
    mesh_renderer.Material->SetAlbedo(glock_albedo);
    mesh_renderer.Material->SetShader(glock_shader);
    glock_entity.AddComponent<MeshRendererComponent>(mesh_renderer);

    auto glock_entity2 = m_ActiveScene->CreateEntity("Glock2");
    auto glock_model2 = AssetManager::LoadModel("C:/dev/zarinApp/assets/models/9mm.obj", (int)glock_entity2);
    auto glock_albedo2 = Texture2D::Create("C:/dev/zarinApp/assets/textures/9mm/GunGS_Albedo.tga.png");
    MeshRendererComponent mesh_renderer2;
    mesh_renderer2.Mesh = glock_model2;
    mesh_renderer2.Material = CreateRef<Material>();
    mesh_renderer2.Material->SetAlbedo(glock_albedo2);
    mesh_renderer2.Material->SetShader(glock_shader);
    glock_entity2.AddComponent<MeshRendererComponent>(mesh_renderer2);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene.get());
}

void EditorLayer::OnDetach() {

}

void EditorLayer::OnUpdate(Timestep ts) {
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) {
        if (m_ViewportSize != m_InitialViewportSize) {
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_InitialViewportSize = m_ViewportSize;
        }

    }
    m_ActiveScene->OnUpdateEditor(ts);

    // Viewport width and mouse position in viewport
    {
        auto[mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        m_ViewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
        m_ViewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
        my = m_ViewportHeight - my;
        m_ViewportMousePosition = { (int)mx, (int)my };
    }

    #ifdef old
    // Resize framebuffers
    auto& framebuffer_specs = m_GBuffer->GetSpecification();
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
    && (framebuffer_specs.Width != m_ViewportSize.x || framebuffer_specs.Height != m_ViewportSize.y)) {
        m_GBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_OutputBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    if (Input::IsKeyPressed(KeyCode::Up)) {
        m_SpherePosition.z -= 5.0f * ts;
    }
    else if (Input::IsKeyPressed(KeyCode::Down)) {
        m_SpherePosition.z += 5.0f * ts;
    }
    if (Input::IsKeyPressed(KeyCode::Left)) {
        m_SpherePosition.x -= 5.0f * ts;
    }
    else if (Input::IsKeyPressed(KeyCode::Right)) {
        m_SpherePosition.x += 5.0f * ts;
    }

    if (m_ViewportFocused)
        m_CameraController.OnUpdate(ts);

    m_Sphere->Transform = glm::translate(glm::mat4(1.0f), m_SpherePosition);
    m_LightSourceCube->Transform = glm::translate(glm::mat4(1.0f), m_LightPosition);

    m_GBuffer->Bind();
    Renderer::BeginScene( m_CameraController.GetCamera() );
    {
        RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
        RenderCommand::Clear();
        
        auto shader = m_ShaderLibrary.Get("GBuffer");
        shader->Bind();
        
        Renderer::Draw(shader, m_Model,  m_ModelTexture);
        Renderer::Draw(shader, m_Sphere, m_ColorTexture);
        Renderer::Draw(shader, m_Floor,  m_FloorDiffuse);
        
        shader = m_ShaderLibrary.Get("FlatColor");
        shader->Bind();
        shader->SetFloat3("u_Color", m_LightColor);

        Renderer::Draw(shader, m_LightSourceCube);
    }
    Renderer::EndScene();
    m_GBuffer->Unbind();

    
    m_OutputBuffer->Bind();
    {
        auto lighting_shader = m_ShaderLibrary.Get("Lighting");
        lighting_shader->Bind();
        m_GBuffer->BindAttachments();
        lighting_shader->SetInt("u_Position",   0);
        lighting_shader->SetInt("u_Normal",     1);
        lighting_shader->SetInt("u_Albedo",     2);
        lighting_shader->SetInt("u_Specular",   3);
        
        lighting_shader->SetFloat3("u_LightColor",     m_LightColor);
        lighting_shader->SetFloat3("u_LightPosition",  m_LightPosition);
        
        // RenderCommand::SetClearColor({ 0.3f, 0.4f, 0.7f, 1.0f });
        // RenderCommand::Clear();
        Renderer::DrawQuad(lighting_shader, m_BufferQuad);
    }
    m_OutputBuffer->Unbind();

    #endif
}

void EditorLayer::OnImGuiRender() {
    ImGuiIO& io = ImGui::GetIO();
    static bool dockspace_open = true;

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    static bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    // fullscreen
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    // Submit the DockSpace
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "", false)) { Application::Get().Close(); }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    {
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewport_panel_size.x, viewport_panel_size.y };

        auto viewport_image = m_ActiveScene->GetOutputImageRendererID();
        ImGui::Image(reinterpret_cast<void*>(viewport_image), viewport_panel_size, ImVec2(0, 1), ImVec2(1, 0));

        // Viewport Bounds
        {
            auto viewport_min_region = ImGui::GetWindowContentRegionMin();
            auto viewport_max_region = ImGui::GetWindowContentRegionMax();
            auto viewport_shift = ImGui::GetWindowPos();
            m_ViewportBounds[0] = { viewport_min_region.x + viewport_shift.x, viewport_min_region.y + viewport_shift.y };
            m_ViewportBounds[1] = { viewport_max_region.x + viewport_shift.x, viewport_max_region.y + viewport_shift.y };
        }

        // Gizmos
        // TODO: add scene hierarchy callbacks
        Entity selected_entity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selected_entity && m_GizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportWidth, m_ViewportHeight);

            // Camera
            const auto& camera = m_ActiveScene->GetViewportCamera();
            const glm::mat4& camera_view = camera.GetView();
            const glm::mat4& camera_projection = camera.GetProjection();
            
            // auto camera_entity = m_ActiveScene->GetPrimaryCameraEntity();
            // const auto& camera = camera_entity.GetComponent<CameraComponent>().Camera;
            // glm::mat4 camera_view = glm::inverse(camera_entity.GetComponent<TransformComponent>().GetTransform());
            // const glm::mat4& camera_projection = camera.GetProjection();

            // Entity transform
            auto& tc = selected_entity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();
            glm::vec3 original_rotation = tc.Rotation;

            bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
            float snap_value = 0.5f;
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snap_value = 15.0f;
            float snap_values[3] = { snap_value, snap_value, snap_value };

            ImGuizmo::Manipulate(
                glm::value_ptr(camera_view), glm::value_ptr(camera_projection),
                (ImGuizmo::OPERATION)(m_GizmoType), ImGuizmo::LOCAL,
                glm::value_ptr(transform),
                nullptr, snap ? snap_values : nullptr
            );

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                Utils::DecomposeTransform(transform, translation, rotation, scale);

                tc.Translation = translation;
                glm::vec3 delta_rotation = rotation - original_rotation;
                tc.Rotation += delta_rotation;
                tc.Scale = scale;
            }
        }
    }
    ImGui::PopStyleVar();
    ImGui::End();

    
    // ImGui::Begin("Output", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    // {
    //     auto& framebuffer = m_ActiveScene->GetOutputBuffer();
    //     auto& spec = framebuffer->GetSpecification();
    //     auto ratio = (float)spec.Width / (float)spec.Height;
    //     auto image_width = ImGui::GetContentRegionAvail().x < 128 ? 128 : ImGui::GetContentRegionAvail().x;
    //     ImVec2 image_size = { image_width, image_width/ratio };
    //     for (size_t i = 0; i < framebuffer->GetColorAttachmentCount(); ++i) {
    //         auto color_attachment_ID = framebuffer->GetColorAttachmentRendererID(i);
    //         ImGui::Image(reinterpret_cast<void*>(color_attachment_ID), image_size, ImVec2(0, 1), ImVec2(1, 0));
    //     }
    //     auto depth_attachment_ID = framebuffer->GetDepthAttachmentRendererID();
    //     ImGui::Image(reinterpret_cast<void*>(depth_attachment_ID), image_size, ImVec2(0, 1), ImVec2(1, 0));
    // }
    // ImGui::End();

    ImGui::End();

    m_SceneHierarchyPanel.OnImGuiRender();
}

void EditorLayer::OnEvent(Event& event) {

    #ifdef old
    m_CameraController.OnEvent(event);
    #endif

    m_ActiveScene->OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(ZRN_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(ZRN_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
    auto keycode = event.GetKeyCode();
    auto repeat_count = event.GetRepeatCount();

    if (m_SceneHierarchyPanel.GetSelectedEntity() && repeat_count == 0) {
        switch (keycode) {
        case KeyCode::t:
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            return true;
        case KeyCode::r:
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            return true;
        case KeyCode::s:
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
            return true;
        case KeyCode::Escape:
            m_GizmoType = -1;
            return true;
        }
    }
    return false;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
    auto button = event.GetMouseButton();

    if (button == MouseButton::Left) {
        if (m_ViewportHovered && !ImGuizmo::IsOver()) {
            m_ActiveScene->GetOutputBuffer()->Bind();
            auto entity = m_ActiveScene->GetEntityOnScreen(m_ViewportMousePosition.x, m_ViewportMousePosition.y);
            m_ActiveScene->GetOutputBuffer()->Unbind();
            m_SceneHierarchyPanel.SelectEntity(entity);
        }
        return true;
    }
    return false;
}

} // namespace zrn