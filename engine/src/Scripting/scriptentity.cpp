#include "Scripting/scriptentity.h"

namespace lua {
ScriptEntity::ScriptEntity(std::string filepath) : m_path(std::move(filepath)) {
    dataBuffer = de::CreateScope<DataBuffer>();
    entityRef = de::CreateScope<LuaEntity>();
    ReloadScript();
}

void ScriptEntity::ReloadScript() {
    int top = Moon::GetTop();
    Moon::LoadFile(m_path.c_str());
    for (char i = 0; i < ScriptFunctions::MaxCount; ++i) {
        m_functions[i].Unload();
        m_functions[i] = Moon::GetGlobalVariable<moon::LuaFunction>(LUA_GLOBAL_FUNCTIONS[i]);
        Moon::CleanGlobalVariable(LUA_GLOBAL_FUNCTIONS[i]);
    }
    DE_ASSERT(top == Moon::GetTop(), "Lua stack is not clean after loading script functions")
}

void ScriptEntity::LoadCodeAndContext() const { Moon::PushGlobalVariable("this", entityRef.get()); }

void ScriptEntity::OnInit() const { Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnInit], dataBuffer.get()); }

void ScriptEntity::OnUpdate(const de::TimeStep& ts) const {
    Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnUpdate], dataBuffer.get(), (float)ts);
}

void ScriptEntity::OnMessage(const std::string& id, const std::string& sender, const moon::LuaDynamicMap& dataToSend) const {
    Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnMessage], dataBuffer.get(), id, sender, dataToSend);
}

void ScriptEntity::OnDestroy() {
    entityRef.reset(nullptr);
    dataBuffer.reset(nullptr);
    for (auto& fn : m_functions) {
        fn.Unload();
    }
}
}  // namespace lua
