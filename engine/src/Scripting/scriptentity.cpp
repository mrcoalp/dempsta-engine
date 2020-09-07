#include "Scripting/scriptentity.h"

#include "Utils/fileutils.h"

namespace lua {
std::string ScriptEntity::s_previousLoadedScript = "";

ScriptEntity::ScriptEntity(const std::string& scriptPath) : m_scriptPath(scriptPath) {
    Data.reset(new lua::DataBuffer());
    EntityRef.reset(new lua::LuaEntity());
}

void ScriptEntity::ReloadScript() { m_scriptCode = de::FileUtils::ReadFile(m_scriptPath); }

void ScriptEntity::LoadCodeAndContext() const {
    LE::PushGlobalVariable("this", EntityRef.get());
    if (s_previousLoadedScript != m_scriptPath) {
        LE::RunCode(m_scriptCode.c_str());
        s_previousLoadedScript = m_scriptPath;
    }
}

void ScriptEntity::OnInit() const {
    LoadCodeAndContext();
    LE::CallFunction("OnInit", Data.get());
}

void ScriptEntity::OnUpdate(const de::TimeStep& ts) const {
    LoadCodeAndContext();
    LE::CallFunction("OnUpdate", Data.get(), (float)ts);
}

void ScriptEntity::OnMessage(const std::string& id, DataBuffer* data, const std::string& sender) const {
    LoadCodeAndContext();
    LE::CallFunction("OnMessage", Data.get(), id, data, sender);
}

void ScriptEntity::OnDestroy() {
    EntityRef.reset(nullptr);
    Data.reset(nullptr);
}
}  // namespace lua
