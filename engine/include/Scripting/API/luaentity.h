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

    LUA_PROXY_METHOD(GetX);

    LUA_PROXY_METHOD(SetX);

    LUA_PROXY_METHOD(GetY);

    LUA_PROXY_METHOD(SetY);

private:
    de::Entity m_entity;
    friend class de::Scene;
};
}  // namespace lua
