#pragma once

#include <glm/glm.hpp>

#include "Scene/entity.h"
#include "Scripting/binding.h"
#include "Scripting/scriptengine.h"

namespace lua {
class LuaEntity {
public:
    explicit LuaEntity() = default;

    explicit LuaEntity(lua_State*) {}

    LUA_DECLARE_CLASS(LuaEntity)

    LUA_METHOD(GetName);
    LUA_METHOD(SetName);
    LUA_METHOD(GetX);
    LUA_METHOD(SetX);
    LUA_METHOD(GetY);
    LUA_METHOD(SetY);
    LUA_METHOD(GetZ);
    LUA_METHOD(SetZ);
    LUA_METHOD(GetColor);
    LUA_METHOD(SetColor);
    LUA_METHOD(GetAlpha);
    LUA_METHOD(SetAlpha);

private:
    de::Entity m_entity;
    friend class de::Scene;
};
}  // namespace lua
