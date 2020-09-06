#include "Scripting/scriptentity.h"

#include "Utils/fileutils.h"

namespace lua {
ScriptEntity::ScriptEntity(const std::string& scriptPath) : m_scriptPath(scriptPath) {}

void ScriptEntity::ReloadScript() { m_scriptCode = de::FileUtils::ReadFile(m_scriptPath); }

void ScriptEntity::SetContext() const { LE::PushGlobalVariable("this", EntityRef.get()); }

void ScriptEntity::LoadCode() const { LE::RunCode(m_scriptCode.c_str()); }

void ScriptEntity::OnInit() const { LE::CallFunction("OnInit", Data.get()); }

void ScriptEntity::OnUpdate(const de::TimeStep& ts) const { LE::CallFunction("OnUpdate", Data.get(), (float)ts); }

void ScriptEntity::OnMessage(const std::string& id, DataBuffer* data, const std::string& sender) const {
    LE::CallFunction("OnMessage", Data.get(), id, data, sender);
}

void ScriptEntity::OnDestroy() {
    EntityRef = nullptr;
    Data = nullptr;
}
}  // namespace lua
