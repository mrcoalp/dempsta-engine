#include "Scripting/API/luaentity.h"

#include "Scene/components.h"

namespace lua {
int LuaEntity::GetX(lua_State* L) {
    auto& transform = m_entity.GetComponent<de::TransformComponent>().Transform;
    SE::PushValue(transform[3][0]);
    return 1;
}

int LuaEntity::SetX(lua_State* L) {
    auto& transform = m_entity.GetComponent<de::TransformComponent>().Transform;
    transform[3][0] = SE::GetValue<float>();
    return 0;
}

int LuaEntity::GetY(lua_State* L) {
    auto& transform = m_entity.GetComponent<de::TransformComponent>().Transform;
    SE::PushValue(transform[3][1]);
    return 1;
}

int LuaEntity::SetY(lua_State* L) {
    auto& transform = m_entity.GetComponent<de::TransformComponent>().Transform;
    transform[3][1] = SE::GetValue<float>();
    return 0;
}

LUA_DEFINE_BINDING(LuaEntity, false)
LUA_ADD_METHOD(GetX)
LUA_ADD_METHOD(SetX)
LUA_ADD_METHOD(GetY)
LUA_ADD_METHOD(SetY)
LUA_ADD_PROPERTY_CUSTOM(x, GetX, SetX)
LUA_ADD_PROPERTY_CUSTOM(y, GetY, SetY);
}  // namespace lua
