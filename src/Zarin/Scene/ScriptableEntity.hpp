#pragma once

#include "Entity.hpp"

namespace zrn {

// Derived class can have OnCreate, OnDestroy, OnUpdate(Timestep) methods
// they will be using accordingly

class ScriptableEntity {
public:
    virtual ~ScriptableEntity() = default;

    template<typename T>
    T& GetComponent() {
        return m_Entity.GetComponent<T>();
    }

friend Scene;
private:
    Entity m_Entity;
};

} // namespace zrn
