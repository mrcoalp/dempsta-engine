#include "Core/assetsmanager.h"

#include "Core/log.h"

namespace de {
AssetsManager& AssetsManager::GetInstance() {
    static AssetsManager instance;
    return instance;
}

AssetsManager::~AssetsManager() {
    for (const auto& asset : m_assets) {
        switch (asset->GetType()) {  // todo(mpinto)
            case AssetType::Sprite:
            case AssetType::Font:
            case AssetType::Sound:
            case AssetType::Script:
            case AssetType::Undefined:
            default:
                break;
        }
    }
}

void AssetsManager::InitFreeType() {
    if (FT_Init_FreeType(&m_fontLibrary) > FT_Err_Ok) {
        LOG_ENGINE_ERROR("An error occurred during FreeType library initialization!");
    }
}

AssetsManager& AssetsManager::AddSprite(const std::string& name, const std::string& filepath) {
    return add(name, filepath, [&filepath]() -> Ref<Asset> { return CreateRef<SpriteAsset>(filepath); });
}

AssetsManager& AssetsManager::AddFont(const std::string& name, const std::string& filepath, unsigned size) {
    return add(name, filepath, [this, &filepath, &size]() -> Ref<Asset> {
        auto font = CreateRef<Font>(m_fontLibrary, filepath, m_fontIndex++, size);
        return CreateRef<FontAsset>(filepath, font);
    });
}

AssetsManager& AssetsManager::AddSound(const std::string& name, const std::string& filepath) {
    return add(name, filepath, [&filepath]() -> Ref<Asset> { return CreateRef<SoundAsset>(filepath); });
}

AssetsManager& AssetsManager::AddScript(const std::string& name, const std::string& filepath) {
    return add(name, filepath, [&filepath]() -> Ref<Asset> { return CreateRef<ScriptAsset>(filepath); });
}

AssetsManager& AssetsManager::AddShader(const std::string& name, const std::string& filepath) {
    return add(name, filepath, [&filepath]() -> Ref<Asset> { return CreateRef<ShaderAsset>(filepath); });
}

const Ref<SubTexture2D>& AssetsManager::GetSprite(const std::string& name) const { return get<SpriteAsset>(name)->GetSprite(); }

const Ref<Font>& AssetsManager::GetFont(const std::string& name) const { return get<FontAsset>(name)->GetFont(); }

Ref<SoundInstance> AssetsManager::GetSoundInstance(const std::string& name) const {
    return SoundInstance::CreateSound(get<SoundAsset>(name)->GetFilePath());
}

const Ref<ScriptAsset>& AssetsManager::GetScript(const std::string& name) const { return get<ScriptAsset>(name); }

const Ref<Shader>& AssetsManager::GetShader(const std::string& name) const { return get<ShaderAsset>(name)->GetShader(); }

AssetsManager& AssetsManager::add(const std::string& name, const std::string& filepath, const std::function<Ref<Asset>()>& assetGetter) {
    if (Exists(name)) {
        LOG_ENGINE_WARN("Tried to add an already loaded asset: {}", name);
        return AssetsManager::GetInstance();
    }
    size_t index;
    if (!sourceLoaded(filepath, index)) {
        m_assets.emplace_back(assetGetter());
    }
    m_tracker.emplace(name, index);
    return AssetsManager::GetInstance();
}

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
