#include "Scripting/scriptentity.h"

#include "Utils/fileutils.h"

namespace lua {
std::string ScriptEntity::s_previousLoadedScript;

ScriptEntity::ScriptEntity(std::string scriptPath) : m_scriptPath(std::move(scriptPath)) {
    dataBuffer = de::CreateScope<DataBuffer>();
    entityRef = de::CreateScope<LuaEntity>();
}

void ScriptEntity::ReloadScript() {
    m_scriptCode = de::FileUtils::ReadFile(m_scriptPath);
    OnInit();
}

void ScriptEntity::SetPath(const std::string& newPath) { m_scriptPath = newPath; }

void ScriptEntity::LoadCodeAndContext() const {
    LE::PushGlobalVariable("this", entityRef.get());
    if (s_previousLoadedScript != m_scriptPath) {
        LE::RunCode(m_scriptCode.c_str());
        s_previousLoadedScript = m_scriptPath;
    }
}

void ScriptEntity::OnInit() const {
    LoadCodeAndContext();
    LE::CallFunction("OnInit", dataBuffer.get());
}

void ScriptEntity::OnUpdate(const de::TimeStep& ts) const {
    LoadCodeAndContext();
    LE::CallFunction("OnUpdate", dataBuffer.get(), (float)ts);
}

void ScriptEntity::OnMessage(const std::string& id, const std::string& sender, const LuaDynamicMap& dataToSend) const {
    LoadCodeAndContext();
    LE::CallFunction("OnMessage", dataBuffer.get(), id, sender, dataToSend);
}

void ScriptEntity::OnDestroy() {
    entityRef.reset(nullptr);
    dataBuffer.reset(nullptr);
}
}  // namespace lua
