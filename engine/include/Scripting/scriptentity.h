#pragma once

#include "Scene/entity.h"

namespace de {
class ScriptEntity {
public:
    virtual ~ScriptEntity() = default;

    template <typename Component>
    Component& GetComponent() {
        return m_entity.GetComponent<Component>();
    }

protected:
    virtual void OnInit() {}

    virtual void OnUpdate(const TimeStep& ts) {}

    virtual void OnDestroy() {}

private:
    Entity m_entity;
    friend class Scene;
};
}  // namespace de
