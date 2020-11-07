#include "Core/assetsmanager.h"

#include "Core/log.h"

namespace de {
AssetsManager& AssetsManager::GetInstance() {
    static AssetsManager instance;
    return instance;
}

void AssetsManager::InitFreeType() {
    if (FT_Init_FreeType(&m_fontLibrary) > FT_Err_Ok) {
        LOG_ENGINE_ERROR("An error occurred during FreeType library initialization!");
    }
}

void AssetsManager::AddSprite(const std::string& name, const std::string& filepath) {
    if (Exists(name)) {
        LOG_ENGINE_WARN("Tried to add an already loaded sprite: '{}'", name);
        return;
    }
    size_t index;
    if (!sourceLoaded(filepath, index)) {
        m_assets.emplace_back(CreateRef<SpriteAsset>(filepath));
    }
    m_tracker.emplace(name, index);
}

void AssetsManager::AddFont(const std::string& name, const std::string& filepath, unsigned size) {
    if (Exists(name)) {
        LOG_ENGINE_WARN("Tried to add an already loaded font: {}", name);
        return;
    }
    size_t index;
    if (!sourceLoaded(filepath, index)) {
        auto font = CreateRef<Font>(m_fontLibrary, filepath, m_fontIndex++, size);
        m_assets.emplace_back(CreateRef<FontAsset>(filepath, font));
    }
    m_tracker.emplace(name, index);
}

const Ref<SubTexture2D>& AssetsManager::GetSprite(const std::string& name) const { return get<SpriteAsset>(name)->GetSprite(); }

const Ref<Font>& AssetsManager::GetFont(const std::string& name) const { return get<FontAsset>(name)->GetFont(); }

bool AssetsManager::sourceLoaded(const std::string& filepath, size_t& index) {
    index = 0;
    for (const auto& asset : m_assets) {
        if (asset->GetFilePath() == filepath) {
            return true;
        }
        ++index;
    }
    return false;
}
}  // namespace de
