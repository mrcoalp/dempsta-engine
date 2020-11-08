#include "Scripting/scriptentity.h"

#include "Utils/fileutils.h"

namespace lua {
std::string ScriptEntity::s_previousLoadedScript;

ScriptEntity::ScriptEntity(std::string filepath) : m_path(std::move(filepath)) {
    dataBuffer = de::CreateScope<DataBuffer>();
    entityRef = de::CreateScope<LuaEntity>();
    ReloadScript();
}

void ScriptEntity::ReloadScript() { m_code = de::FileUtils::ReadFile(m_path); }

void ScriptEntity::LoadCodeAndContext() const {
    LE::PushGlobalVariable("this", entityRef.get());
    if (s_previousLoadedScript != m_path) {
        LE::RunCode(m_code.c_str());
        s_previousLoadedScript = m_path;
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
