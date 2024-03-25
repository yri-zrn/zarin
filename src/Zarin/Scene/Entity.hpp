#pragma once

#include "Scene/Scene.hpp"

#include <entt.hpp>

namespace zrn {

class Entity {

public:
    Entity() = default;
    Entity(const Entity& other) = default;
    ~Entity() = default;

    // should it be private?
    Entity(entt::entity entity, Scene* scene);

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        ZRN_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component");
        T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template<typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args) {
        T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
        ///
        return component;
    }

    template<typename T>
    T& GetComponent() {
        ZRN_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template<typename T>
    void RemoveComponent() {
        ZRN_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
    operator entt::entity() const { return m_EntityHandle; }
    operator uint32_t() const { return (uint32_t)m_EntityHandle; }
    operator int() const { return (int)m_EntityHandle; }

    const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

    bool operator==(const Entity& other) const {
        return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

// friend Scene;
private:
    entt::entity m_EntityHandle{ entt::null };
    Scene* m_Scene = nullptr;

    // Should using intrusive reference countig pointer
    // std::weak_ref<Scene> m_Scene;

};

} // namespace zrn
