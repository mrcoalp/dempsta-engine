#include "Scripting/API/luaentity.h"

#include "Renderer/color.h"
#include "Scene/components.h"
#include "Scripting/messaging.h"

namespace lua {
int LuaEntity::GetName(lua_State* L) {
    LE::PushValue(m_entity.GetComponent<de::NameComponent>().name);
    return 1;
}

int LuaEntity::SetName(lua_State* L) {
    m_entity.GetComponent<de::NameComponent>().name = LE::GetValue<std::string>();
    return 0;
}

int LuaEntity::GetX(lua_State* L) {
    LE::PushValue(m_entity.GetComponent<de::TransformComponent>().transform[3][0]);
    return 1;
}

int LuaEntity::SetX(lua_State* L) {
    m_entity.GetComponent<de::TransformComponent>().transform[3][0] = LE::GetValue<float>();
    return 0;
}

int LuaEntity::GetY(lua_State* L) {
    LE::PushValue(m_entity.GetComponent<de::TransformComponent>().transform[3][1]);
    return 1;
}

int LuaEntity::SetY(lua_State* L) {
    m_entity.GetComponent<de::TransformComponent>().transform[3][1] = LE::GetValue<float>();
    return 0;
}

int LuaEntity::GetZ(lua_State* L) {
    LE::PushValue(m_entity.GetComponent<de::TransformComponent>().transform[3][2]);
    return 1;
}

int LuaEntity::SetZ(lua_State* L) {
    m_entity.GetComponent<de::TransformComponent>().transform[3][2] = LE::GetValue<float>();
    return 0;
}

int LuaEntity::GetScale(lua_State* L) {
    const auto& transform = m_entity.GetComponent<de::TransformComponent>().transform;
    std::unordered_map<std::string, float> scale = {
        {"x", transform[0][0]}, {"y", transform[1][1]}, {"z", transform[2][2]}};
    LE::PushValue(scale);
    return 1;
}

int LuaEntity::SetScale(lua_State* L) {
    auto& transform = m_entity.GetComponent<de::TransformComponent>().transform;
    auto scale = LE::GetMap<float>({"x", "y", "z"});
    transform[0][0] = scale["x"];
    transform[1][1] = scale["y"];
    transform[2][2] = scale["z"];
    return 0;
}

int LuaEntity::GetColor(lua_State* L) {
    LE::PushValue(de::Color::Vec4ToHexString(m_entity.GetComponent<de::SpriteComponent>().color));
    return 1;
}

int LuaEntity::SetColor(lua_State* L) {
    m_entity.GetComponent<de::SpriteComponent>().color = de::Color::HexStringToVec4(LE::GetValue<std::string>());
    return 0;
}

int LuaEntity::GetAlpha(lua_State* L) {
    LE::PushValue(static_cast<int>(m_entity.GetComponent<de::SpriteComponent>().color.a * 255.f));
    return 1;
}

int LuaEntity::SetAlpha(lua_State* L) {
    m_entity.GetComponent<de::SpriteComponent>().color.a = LE::GetValue<float>() / 255.f;
    return 0;
}

int LuaEntity::GetAcquireEvents(lua_State* L) {
    LE::PushValue(m_entity.GetComponent<de::ScriptComponent>().instance->AcquireEvents);
    return 1;
}

int LuaEntity::SetAcquireEvents(lua_State* L) {
    m_entity.GetComponent<de::ScriptComponent>().instance->AcquireEvents = LE::GetValue<bool>();
    return 0;
}

int LuaEntity::SendMessage(lua_State* L) {
    auto name = LE::GetValue<std::string>();
    auto* data = *LE::GetValue<DataBuffer**>(2);
    auto sender = m_entity.GetComponent<de::NameComponent>().name;
    MessageHandler::AddMessage({name, data, sender});
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
LUA_ADD_METHOD(GetScale)
LUA_ADD_METHOD(SetScale)
LUA_ADD_PROPERTY_CUSTOM(scale, GetScale, SetScale)
LUA_ADD_METHOD(GetColor)
LUA_ADD_METHOD(SetColor)
LUA_ADD_PROPERTY_CUSTOM(color, GetColor, SetColor)
LUA_ADD_METHOD(GetAlpha)
LUA_ADD_METHOD(SetAlpha)
LUA_ADD_PROPERTY_CUSTOM(alpha, GetAlpha, SetAlpha)
LUA_ADD_PROPERTY_CUSTOM(acquire_events, GetAcquireEvents, SetAcquireEvents)
LUA_ADD_METHOD(SendMessage);
}  // namespace lua
