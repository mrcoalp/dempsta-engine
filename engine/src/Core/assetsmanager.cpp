#include "Core/assetsmanager.h"

#include "Core/log.h"

namespace de {
AssetsManager& AssetsManager::GetInstance() {
    static AssetsManager instance;
    return instance;
}

void AssetsManager::AddSprite(const std::string& name, const std::string& filepath) {
    if (Exists(name)) {
        LOG_ENGINE_WARN("Tried to add an already existing asset with name: '{}'", name);
        return;
    }
    bool exists = false;
    size_t index = 0;
    for (const auto& asset : m_assets) {
        if (asset->GetFilePath() == filepath) {
            exists = true;
            break;
        }
        ++index;
    }
    if (!exists) {
        m_assets.emplace_back(CreateRef<SpriteAsset>(filepath));
    }
    m_tracker.emplace(name, index);
}

const Ref<SubTexture2D>& AssetsManager::GetSprite(const std::string& name) const { return get<SpriteAsset>(name)->GetSprite(); }
}  // namespace de
