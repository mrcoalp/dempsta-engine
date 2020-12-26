#include "Scripting/scriptentity.h"

namespace lua {
ScriptEntity::ScriptEntity(std::string filepath, const de::Entity& entity) : m_path(std::move(filepath)) {
    m_data = Moon::CreateDynamicMap();
    Moon::PushValue(new LuaEntity(entity));
    m_entity = Moon::CreateRef();
    Moon::Pop();  // Pop LuaEntity from stack, since it is already stored as ref
}

void ScriptEntity::ReloadScript() {
    int top = Moon::GetTop();
    Moon::LoadFile(m_path.c_str());
    for (char i = 0; i < ScriptFunctions::MaxCount; ++i) {
        m_functions[i].Unload();
        m_functions[i] = Moon::GetGlobalVariable<moon::LuaFunction>(LUA_GLOBAL_FUNCTIONS[i]);
        Moon::CleanGlobalVariable(LUA_GLOBAL_FUNCTIONS[i]);
    }
    DE_ASSERT(top == Moon::GetTop(), "Lua stack is not clean after loading script functions!")
}

void ScriptEntity::LoadCodeAndContext() const { Moon::PushGlobalVariable("this", m_entity); }

void ScriptEntity::OnInit() const { Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnInit], m_data); }

void ScriptEntity::OnUpdate(const de::TimeStep& ts) const { Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnUpdate], m_data, (float)ts); }

void ScriptEntity::OnMessage(const std::string& id, const std::string& sender, const moon::LuaDynamicMap& dataToSend) const {
    Moon::LuaFunctionCall(m_functions[ScriptFunctions::OnMessage], m_data, id, sender, dataToSend);
}

void ScriptEntity::OnDestroy() {
    m_data.Unload();
    m_entity.Unload();
    for (auto& fn : m_functions) {
        fn.Unload();
    }
}
}  // namespace lua
