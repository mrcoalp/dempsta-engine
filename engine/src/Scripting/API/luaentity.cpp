#include "Scripting/API/luaentity.h"

#include "Core/math.h"
#include "Renderer/color.h"
#include "Scene/components.h"
#include "Scripting/messaging.h"

namespace lua {
int LuaEntity::GetName(lua_State*) {
    CHECK_GETTER(de::NameComponent)
    Moon::PushValue(m_entity.GetComponent<de::NameComponent>().name);
    return 1;
}

int LuaEntity::SetName(lua_State*) {
    CHECK_SETTER(de::NameComponent)
    m_entity.GetComponent<de::NameComponent>().name = Moon::GetValue<std::string>();
    return 0;
}

int LuaEntity::GetX(lua_State*) {
    CHECK_GETTER(de::TransformComponent)
    Moon::PushValue(m_entity.GetComponent<de::TransformComponent>().translation.x);
    return 1;
}

int LuaEntity::SetX(lua_State*) {
    CHECK_SETTER(de::TransformComponent)
    m_entity.GetComponent<de::TransformComponent>().translation.x = Moon::GetValue<float>();
    return 0;
}

int LuaEntity::GetY(lua_State*) {
    CHECK_GETTER(de::TransformComponent)
    Moon::PushValue(m_entity.GetComponent<de::TransformComponent>().translation.y);
    return 1;
}

int LuaEntity::SetY(lua_State*) {
    CHECK_SETTER(de::TransformComponent)
    m_entity.GetComponent<de::TransformComponent>().translation.y = Moon::GetValue<float>();
    return 0;
}

int LuaEntity::GetZ(lua_State*) {
    CHECK_GETTER(de::TransformComponent)
    Moon::PushValue(m_entity.GetComponent<de::TransformComponent>().translation.z);
    return 1;
}

int LuaEntity::SetZ(lua_State*) {
    CHECK_SETTER(de::TransformComponent)
    m_entity.GetComponent<de::TransformComponent>().translation.z = Moon::GetValue<float>();
    return 0;
}

int LuaEntity::GetPosition(lua_State*) {
    CHECK_GETTER(de::TransformComponent)
    const auto& translation = m_entity.GetComponent<de::TransformComponent>().translation;
    std::unordered_map<std::string, float> translationMap = {{"x", translation.x}, {"y", translation.y}, {"z", translation.z}};
    Moon::PushValue(translationMap);
    return 1;
}

int LuaEntity::SetPosition(lua_State*) {
    CHECK_SETTER(de::TransformComponent)
    auto translationMap = Moon::GetValue<moon::LuaMap<float>>();
    if (!Moon::EnsureMapKeys({"x", "y", "z"}, translationMap)) {
        LOG_ENGINE_WARN("Position map must contain: 'x' 'y' 'z' keys!");
        return 0;
    }
    m_entity.GetComponent<de::TransformComponent>().translation = {translationMap.at("x"), translationMap.at("y"), translationMap.at("z")};
    return 0;
}

int LuaEntity::GetScale(lua_State*) {
    CHECK_GETTER(de::TransformComponent)
    const auto& scale = m_entity.GetComponent<de::TransformComponent>().scale;
    std::unordered_map<std::string, float> scaleMap = {{"x", scale.x}, {"y", scale.y}, {"z", scale.z}};
    Moon::PushValue(scaleMap);
    return 1;
}

int LuaEntity::SetScale(lua_State*) {
    CHECK_SETTER(de::TransformComponent)
    auto scaleMap = Moon::GetValue<moon::LuaMap<float>>();
    if (!Moon::EnsureMapKeys({"x", "y", "z"}, scaleMap)) {
        LOG_ENGINE_WARN("Scale map must contain: 'x' 'y' 'z' keys!");
        return 0;
    }
    m_entity.GetComponent<de::TransformComponent>().scale = {scaleMap.at("x"), scaleMap.at("y"), scaleMap.at("z")};
    return 0;
}

int LuaEntity::GetColor(lua_State*) {
    CHECK_GETTER(de::SpriteComponent)
    Moon::PushValue(de::Color::Vec4ToHexString(m_entity.GetComponent<de::SpriteComponent>().color));
    return 1;
}

int LuaEntity::SetColor(lua_State*) {
    CHECK_SETTER(de::SpriteComponent)
    m_entity.GetComponent<de::SpriteComponent>().color = de::Color::HexStringToVec4(Moon::GetValue<std::string>());
    return 0;
}

int LuaEntity::GetAlpha(lua_State*) {
    CHECK_GETTER(de::SpriteComponent)
    Moon::PushValue(static_cast<int>(m_entity.GetComponent<de::SpriteComponent>().color.a * 255.f));
    return 1;
}

int LuaEntity::SetAlpha(lua_State*) {
    CHECK_SETTER(de::SpriteComponent)
    m_entity.GetComponent<de::SpriteComponent>().color.a = Moon::GetValue<float>() / 255.f;
    return 0;
}

int LuaEntity::PlaySound(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->Play();
    return 0;
}

int LuaEntity::PauseSound(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->Pause();
    return 0;
}

int LuaEntity::ResumeSound(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->Resume();
    return 0;
}

int LuaEntity::StopSound(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->Stop();
    return 0;
}

int LuaEntity::GetGain(lua_State*) {
    CHECK_GETTER(de::SoundComponent)
    Moon::PushValue(m_entity.GetComponent<de::SoundComponent>().sound->GetGain());
    return 1;
}

int LuaEntity::SetGain(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->SetGain(Moon::GetValue<float>());
    return 0;
}

int LuaEntity::GetPitch(lua_State*) {
    CHECK_GETTER(de::SoundComponent)
    Moon::PushValue(m_entity.GetComponent<de::SoundComponent>().sound->GetPitch());
    return 1;
}

int LuaEntity::SetPitch(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->SetPitch(Moon::GetValue<float>());
    return 0;
}

int LuaEntity::GetPan(lua_State*) {
    CHECK_GETTER(de::SoundComponent)
    Moon::PushValue(m_entity.GetComponent<de::SoundComponent>().sound->GetPan());
    return 1;
}

int LuaEntity::SetPan(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->SetPan(Moon::GetValue<float>());
    return 0;
}

int LuaEntity::GetLooped(lua_State*) {
    CHECK_GETTER(de::SoundComponent)
    Moon::PushValue(m_entity.GetComponent<de::SoundComponent>().sound->GetLooped());
    return 1;
}

int LuaEntity::SetLooped(lua_State*) {
    CHECK_SETTER(de::SoundComponent)
    m_entity.GetComponent<de::SoundComponent>().sound->SetLooped(Moon::GetValue<bool>());
    return 0;
}

int LuaEntity::GetAcquireEvents(lua_State*) {
    CHECK_GETTER(de::ScriptComponent)
    Moon::PushValue(m_entity.GetComponent<de::ScriptComponent>().instance->acquireEvents);
    return 1;
}

int LuaEntity::SetAcquireEvents(lua_State*) {
    CHECK_SETTER(de::ScriptComponent)
    m_entity.GetComponent<de::ScriptComponent>().instance->acquireEvents = Moon::GetValue<bool>();
    return 0;
}

int LuaEntity::SendMessage(lua_State*) {
    CHECK_SETTER(de::NameComponent)
    auto id = Moon::GetValue<std::string>();
    auto sender = m_entity.GetComponent<de::NameComponent>().name;
    if (Moon::GetTop() == 1) {  // In case no data was sent, only one argument (id)
        MessageHandler::AddMessage({id, sender, moon::LuaDynamicMap(Moon::GetState())});
        return 0;
    }
    auto data = Moon::GetValue<moon::LuaDynamicMap>(2);
    MessageHandler::AddMessage({id, sender, data});
    return 0;
}

MOON_DEFINE_BINDING(LuaEntity, false)
MOON_ADD_METHOD(GetName)
MOON_ADD_METHOD(SetName)
MOON_ADD_PROPERTY_CUSTOM(name, GetName, SetName)
MOON_ADD_METHOD(GetX)
MOON_ADD_METHOD(SetX)
MOON_ADD_PROPERTY_CUSTOM(x, GetX, SetX)
MOON_ADD_METHOD(GetY)
MOON_ADD_METHOD(SetY)
MOON_ADD_PROPERTY_CUSTOM(y, GetY, SetY)
MOON_ADD_METHOD(GetZ)
MOON_ADD_METHOD(SetZ)
MOON_ADD_PROPERTY_CUSTOM(z, GetZ, SetZ)
MOON_ADD_METHOD(GetPosition)
MOON_ADD_METHOD(SetPosition)
MOON_ADD_PROPERTY_CUSTOM(position, GetPosition, SetPosition)
MOON_ADD_METHOD(GetScale)
MOON_ADD_METHOD(SetScale)
MOON_ADD_PROPERTY_CUSTOM(scale, GetScale, SetScale)
MOON_ADD_METHOD(GetColor)
MOON_ADD_METHOD(SetColor)
MOON_ADD_PROPERTY_CUSTOM(color, GetColor, SetColor)
MOON_ADD_METHOD(GetAlpha)
MOON_ADD_METHOD(SetAlpha)
MOON_ADD_METHOD(PlaySound)
MOON_ADD_METHOD(PauseSound)
MOON_ADD_METHOD(ResumeSound)
MOON_ADD_METHOD(StopSound)
MOON_ADD_PROPERTY_CUSTOM(gain, GetGain, SetGain)
MOON_ADD_PROPERTY_CUSTOM(volume, GetGain, SetGain)
MOON_ADD_PROPERTY_CUSTOM(pitch, GetPitch, SetPitch)
MOON_ADD_PROPERTY_CUSTOM(pan, GetPan, SetPan)
MOON_ADD_PROPERTY_CUSTOM(looped, GetLooped, SetLooped)
MOON_ADD_PROPERTY_CUSTOM(alpha, GetAlpha, SetAlpha)
MOON_ADD_PROPERTY_CUSTOM(acquire_events, GetAcquireEvents, SetAcquireEvents)
MOON_ADD_METHOD(SendMessage);
}  // namespace lua
