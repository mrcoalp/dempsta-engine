#include "Scripting/API/luaentity.h"

#include "Renderer/color.h"
#include "Scene/components.h"

namespace lua {
int LuaEntity::GetName(lua_State* L) {
    SE::PushValue(m_entity.GetComponent<de::NameComponent>().Name);
    return 1;
}

int LuaEntity::SetName(lua_State* L) {
    m_entity.GetComponent<de::NameComponent>().Name = SE::GetValue<std::string>();
    return 0;
}

int LuaEntity::GetX(lua_State* L) {
    SE::PushValue(m_entity.GetComponent<de::TransformComponent>().Transform[3][0]);
    return 1;
}

int LuaEntity::SetX(lua_State* L) {
    m_entity.GetComponent<de::TransformComponent>().Transform[3][0] = SE::GetValue<float>();
    return 0;
}

int LuaEntity::GetY(lua_State* L) {
    SE::PushValue(m_entity.GetComponent<de::TransformComponent>().Transform[3][1]);
    return 1;
}

int LuaEntity::SetY(lua_State* L) {
    m_entity.GetComponent<de::TransformComponent>().Transform[3][1] = SE::GetValue<float>();
    return 0;
}

int LuaEntity::GetZ(lua_State* L) {
    SE::PushValue(m_entity.GetComponent<de::TransformComponent>().Transform[3][2]);
    return 1;
}

int LuaEntity::SetZ(lua_State* L) {
    m_entity.GetComponent<de::TransformComponent>().Transform[3][2] = SE::GetValue<float>();
    return 0;
}

int LuaEntity::GetColor(lua_State* L) {
    SE::PushValue(de::Color::Vec4ToHexString(m_entity.GetComponent<de::SpriteComponent>().Color));
    return 1;
}

int LuaEntity::SetColor(lua_State* L) {
    m_entity.GetComponent<de::SpriteComponent>().Color = de::Color::HexStringToVec4(SE::GetValue<std::string>());
    return 0;
}

int LuaEntity::GetAlpha(lua_State* L) {
    SE::PushValue(static_cast<int>(m_entity.GetComponent<de::SpriteComponent>().Color.a * 255.f));
    return 1;
}

int LuaEntity::SetAlpha(lua_State* L) {
    m_entity.GetComponent<de::SpriteComponent>().Color.a = SE::GetValue<float>() / 255.f;
    return 0;
}

LUA_DEFINE_BINDING(LuaEntity, false)
LUA_ADD_METHOD(GetName)
LUA_ADD_METHOD(SetName)
LUA_ADD_PROPERTY_CUSTOM(name, GetName, SetName)
LUA_ADD_METHOD(GetX)
LUA_ADD_METHOD(SetX)
LUA_ADD_PROPERTY_CUSTOM(x, GetX, SetX)
LUA_ADD_METHOD(GetY)
LUA_ADD_METHOD(SetY)
LUA_ADD_PROPERTY_CUSTOM(y, GetY, SetY)
LUA_ADD_METHOD(GetZ)
LUA_ADD_METHOD(SetZ)
LUA_ADD_PROPERTY_CUSTOM(z, GetZ, SetZ)
LUA_ADD_METHOD(GetColor)
LUA_ADD_METHOD(SetColor)
LUA_ADD_PROPERTY_CUSTOM(color, GetColor, SetColor)
LUA_ADD_METHOD(GetAlpha)
LUA_ADD_METHOD(SetAlpha)
LUA_ADD_PROPERTY_CUSTOM(alpha, GetAlpha, SetAlpha);
}  // namespace lua
