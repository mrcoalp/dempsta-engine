#pragma once

#include <moon.h>

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
        LoadCodeAndContext();
        Moon::CallFunction("OnEvent", dataBuffer.get(), eventType, action);
    }

    void OnMessage(const std::string& id, const std::string& sender, const moon_types::LuaDynamicMap& data) const;

    void OnDestroy();

private:
    std::string m_path;
    std::string m_code;

    static std::string s_previousLoadedScript;
};
}  // namespace lua
