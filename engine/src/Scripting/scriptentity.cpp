#include "Scripting/scriptentity.h"

#include "Core/assetsmanager.h"

namespace lua {
std::string ScriptEntity::s_previousLoadedScript;

ScriptEntity::ScriptEntity(std::string asset) : m_asset(std::move(asset)) {
    dataBuffer = de::CreateScope<DataBuffer>();
    entityRef = de::CreateScope<LuaEntity>();
}

void ScriptEntity::ReloadScript() {
    de::AssetsManager::GetInstance().GetScript(m_asset)->Reload();
    OnInit();
}

void ScriptEntity::SetAsset(const std::string& asset) {
    if (!de::AssetsManager::GetInstance().IsScript(asset)) {
        return;
    }
    m_asset = asset;
    ReloadScript();
}

void ScriptEntity::LoadCodeAndContext() const {
    LE::PushGlobalVariable("this", entityRef.get());
    const auto& assets = de::AssetsManager::GetInstance();
    if (s_previousLoadedScript != assets.GetScript(m_asset)->GetFilePath()) {
        LE::RunCode(assets.GetScript(m_asset)->GetCode().c_str());
        s_previousLoadedScript = assets.GetScript(m_asset)->GetFilePath();
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
