#pragma once

#include "Core/core.h"
#include "Scene/entity.h"
#include "Scripting/API/databuffer.h"
#include "Scripting/API/luaentity.h"
#include "Scripting/luaengine.h"

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
    explicit ScriptEntity(std::string scriptPath);

    de::Scope<DataBuffer> dataBuffer;
    de::Scope<LuaEntity> entityRef;
    bool acquireEvents = false;

    void ReloadScript();

    [[nodiscard]] inline const std::string& GetPath() const { return m_scriptPath; }

    void LoadCodeAndContext() const;

    void OnInit() const;

    void OnUpdate(const de::TimeStep& ts) const;

    template <typename T>
    void OnEvent(int eventType, T&& action) const {
        LoadCodeAndContext();
        LE::CallFunction("OnEvent", dataBuffer.get(), eventType, action);
    }

    void OnMessage(const std::string& id, DataBuffer* data, const std::string& sender) const;

    void OnDestroy();

private:
    std::string m_scriptPath;
    std::string m_scriptCode;

    static std::string s_previousLoadedScript;
};
}  // namespace lua
