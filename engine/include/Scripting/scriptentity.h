#pragma once

#include <moon/moon.h>

#include "Core/core.h"
#include "Scene/entity.h"
#include "Scripting/API/databuffer.h"
#include "Scripting/API/luaentity.h"

namespace de {
class NativeScriptEntity {
public:
    virtual ~NativeScriptEntity() = default;

    template <typename Component>
    Component& GetComponent() {
        return m_entity.GetComponent<Component>();
    }

protected:
    virtual void OnInit() {}

    virtual void OnUpdate(const TimeStep& ts) {}

    virtual void OnEvent(Event& event) {}

    virtual void OnDestroy() {}

private:
    Entity m_entity;
    friend class Scene;
};
}  // namespace de

namespace lua {
enum ScriptFunctions { OnInit, OnUpdate, OnEvent, OnMessage, MaxCount };
constexpr const char* LUA_GLOBAL_FUNCTIONS[ScriptFunctions::MaxCount]{"OnInit", "OnUpdate", "OnEvent", "OnMessage"};

class ScriptEntity {
public:
    explicit ScriptEntity(std::string filepath);

    de::Scope<DataBuffer> dataBuffer;
    de::Scope<LuaEntity> entityRef;
    bool acquireEvents = false;

    inline void SetPath(const std::string& filepath) { m_path = filepath; }

    void ReloadScript();

    void LoadCodeAndContext() const;

    void OnInit() const;

    void OnUpdate(const de::TimeStep& ts) const;

    template <typename T>
    void OnEvent(int eventType, T&& action) const {
        Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnEvent], dataBuffer.get(), eventType, action);
    }

    void OnMessage(const std::string& id, const std::string& sender, const moon::LuaDynamicMap& data) const;

    void OnDestroy();

private:
    std::string m_path;
    moon::LuaFunction m_functions[ScriptFunctions::MaxCount];
};
}  // namespace lua
