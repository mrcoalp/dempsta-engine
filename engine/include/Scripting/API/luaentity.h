#pragma once

#include <moon/moon.h>

#include <glm/glm.hpp>

#include "Scene/entity.h"

#ifndef CHECK_GETTER
#define CHECK_GETTER(_component)                                                \
    if (!m_entity.HasComponent<_component>()) {                                 \
        LOG_ENGINE_WARN("Component {} is not present in entity!", #_component); \
        Moon::PushNull();                                                       \
        return 1;                                                               \
    }
#endif

#ifndef CHECK_SETTER
#define CHECK_SETTER(_component)                                                \
    if (!m_entity.HasComponent<_component>()) {                                 \
        LOG_ENGINE_WARN("Component {} is not present in entity!", #_component); \
        return 0;                                                               \
    }
#endif

namespace lua {
class LuaEntity {
public:
    explicit LuaEntity(de::Entity entity) : m_entity(std::move(entity)) {}

    explicit LuaEntity(lua_State*) {}

    MOON_DECLARE_CLASS(LuaEntity)

    MOON_METHOD(GetName);
    MOON_METHOD(SetName);
    // Transform
    MOON_METHOD(GetX);
    MOON_METHOD(SetX);
    MOON_METHOD(GetY);
    MOON_METHOD(SetY);
    MOON_METHOD(GetZ);
    MOON_METHOD(SetZ);
    MOON_METHOD(GetPosition);
    MOON_METHOD(SetPosition);
    MOON_METHOD(GetScale);
    MOON_METHOD(SetScale);
    MOON_METHOD(GetColor);
    MOON_METHOD(SetColor);
    MOON_METHOD(GetAlpha);
    MOON_METHOD(SetAlpha);
    // Sound
    MOON_METHOD(PlaySound);
    MOON_METHOD(PauseSound);
    MOON_METHOD(ResumeSound);
    MOON_METHOD(StopSound);
    MOON_METHOD(GetGain);
    MOON_METHOD(SetGain);
    MOON_METHOD(GetPitch);
    MOON_METHOD(SetPitch);
    MOON_METHOD(GetPan);
    MOON_METHOD(SetPan);
    MOON_METHOD(GetLooped);
    MOON_METHOD(SetLooped);

    MOON_METHOD(GetAcquireEvents);
    MOON_METHOD(SetAcquireEvents);
    MOON_METHOD(SendMessage);

private:
    de::Entity m_entity;
};
}  // namespace lua
