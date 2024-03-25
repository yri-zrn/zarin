#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Zarin/Core/Timestep.hpp"
#include "Zarin/Renderer/Mesh.hpp"
#include "Zarin/Renderer/Shader.hpp"
#include "Zarin/Renderer/Texture.hpp"
#include "Zarin/Renderer/Material.hpp"

#include "SceneCamera.hpp"

#include <concepts>
#include <type_traits>

namespace zrn {

struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag) { }
};

struct TransformComponent {
    glm::vec3 Translation { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation { 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent& other) = default;
    TransformComponent(const glm::vec3& translation)
        : Translation(translation) { }
    
    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Translation)
             * rotation
             * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct MeshRendererComponent {
    Ref<zrn::Mesh>     Mesh;
    Ref<zrn::Material> Material;

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent& other) = default;
};

struct CameraComponent {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent() = default;
    CameraComponent(const CameraComponent& other) = default;
};

class ScriptableEntity;

// #define HAS_METHOD(f)                                       \
// template<typename T>                                        \
// concept has_##f = requires(T&& scriptable_entity) {    \
//     { scriptable_entity.f() } -> std::same_as<void>; \
// };                                                          \
// HAS_METHOD_CONCEPT(OnCreate)
// HAS_METHOD_CONCEPT(OnDestroy)

template<typename T>
concept has_OnCreate = requires(T&& scriptable_entity) {
    { scriptable_entity.OnCreate() } -> std::same_as<void>;
};

template<typename T>
concept has_OnDestroy = requires(T&& scriptable_entity) {
    { scriptable_entity.OnDestroy() } -> std::same_as<void>;
};

template<typename T>
concept has_OnUpdate = requires(T&& scriptable_entity, Timestep ts) {
    { scriptable_entity.OnUpdate(ts) } -> std::same_as<void>;
};

template<typename T>
concept has_OnEvent = requires(T&& scriptable_entity) {
    { scriptable_entity.OnEvent() } -> std::same_as<void>;
};

template <typename T>
concept scriptable_entity_child = std::is_base_of_v<ScriptableEntity, T>;

struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;
    
    // void (*Construct)();
    // void (*Destroy)();
    // void (*OnCreate)();
    // void (*OnDestroy)();
    // void (*OnUpdate)(Timestep);
    // void (*OnEvent)();

    void (*InstantiateScript)(NativeScriptComponent*);
    void (*DestroyScript)(NativeScriptComponent*);

    // Script methods
    void (*OnCreate)(ScriptableEntity*);
    void (*OnDestroy)(ScriptableEntity*);
    void (*OnUpdate)(ScriptableEntity*, Timestep);
    void (*OnEvent)(ScriptableEntity*);

    template<scriptable_entity_child T>
    void Bind() {
        // TODO: replace with captured lambdas?
        InstantiateScript = [](NativeScriptComponent* nsc) { nsc->Instance = static_cast<ScriptableEntity*>(new T()); };
        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };

        if constexpr (has_OnCreate<T>)
            OnCreate = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
        
        if constexpr (has_OnDestroy<T>)
            OnDestroy = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };

        if constexpr (has_OnUpdate<T>)
            OnUpdate = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };

        if constexpr (has_OnEvent<T>)
            OnDestroy = [](ScriptableEntity* instance) { ((T*)instance)->OnEvent(); };
    }
};

} // namespace zrn
