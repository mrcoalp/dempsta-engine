#pragma once

#include <glm/glm.hpp>

#include "Scene/entity.h"
#include "Scripting/binding.h"
#include "Scripting/luaengine.h"

namespace lua {
class LuaEntity {
public:
    explicit LuaEntity() = default;

    explicit LuaEntity(lua_State*) {}

    LUA_DECLARE_CLASS(LuaEntity)

    LUA_METHOD(GetName);
    LUA_METHOD(SetName);
    // Transform
    LUA_METHOD(GetX);
    LUA_METHOD(SetX);
    LUA_METHOD(GetY);
    LUA_METHOD(SetY);
    LUA_METHOD(GetZ);
    LUA_METHOD(SetZ);
    LUA_METHOD(GetScale);
    LUA_METHOD(SetScale);
    LUA_METHOD(GetColor);
    LUA_METHOD(SetColor);
    LUA_METHOD(GetAlpha);
    LUA_METHOD(SetAlpha);
    // Sound
    LUA_METHOD(PlaySound);
    LUA_METHOD(PauseSound);
    LUA_METHOD(ResumeSound);
    LUA_METHOD(StopSound);
    LUA_METHOD(GetGain);
    LUA_METHOD(SetGain);
    LUA_METHOD(GetPitch);
    LUA_METHOD(SetPitch);
    LUA_METHOD(GetPan);
    LUA_METHOD(SetPan);
    LUA_METHOD(GetLooped);
    LUA_METHOD(SetLooped);

    LUA_METHOD(GetAcquireEvents);
    LUA_METHOD(SetAcquireEvents);
    LUA_METHOD(SendMessage);

private:
    de::Entity m_entity;
    friend class de::Scene;

    template <typename Component>
    bool check_component() {
        return m_entity.HasComponent<Component>();
    }
};
}  // namespace lua
