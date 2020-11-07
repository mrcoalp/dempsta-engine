#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Core/core.h"
#include "Renderer/subtexture.h"

namespace de {
enum class AssetType { Undefined, Sprite, Font, Sound, Script };

class Asset {
public:
    explicit Asset(std::string filepath) : m_filepath(std::move(filepath)) {}
    virtual ~Asset() = default;

    [[nodiscard]] const std::string& GetFilePath() const { return m_filepath; }

protected:
    AssetType m_type{AssetType::Undefined};

private:
    std::string m_filepath;
};
class SpriteAsset : public Asset {
public:
    explicit SpriteAsset(const std::string& filepath) : Asset(filepath) {
        m_type = AssetType::Sprite;
        m_sprite = SubTexture2D::CreateSprite(filepath);
    }

    [[nodiscard]] const Ref<SubTexture2D>& GetSprite() const { return m_sprite; }

private:
    Ref<SubTexture2D> m_sprite;
};
struct FontAsset : public Asset {
    explicit FontAsset(const std::string& filepath) : Asset(filepath) { m_type = AssetType::Font; }
};
struct SoundAsset : public Asset {
    explicit SoundAsset(const std::string& filepath) : Asset(filepath) { m_type = AssetType::Sound; }
};
struct ScriptAsset : public Asset {
    explicit ScriptAsset(const std::string& filepath) : Asset(filepath) { m_type = AssetType::Script; }
};

class AssetsManager {
public:
    static AssetsManager& GetInstance();

    [[nodiscard]] inline bool Exists(const std::string& name) const { return m_tracker.find(name) != m_tracker.end(); }

    void AddSprite(const std::string& name, const std::string& filepath);

    [[nodiscard]] const Ref<SubTexture2D>& GetSprite(const std::string& name) const;

private:
    AssetsManager() = default;

    template <class T>
    const Ref<T>& get(const std::string& name) const {
        if (!Exists(name)) {
            DE_THROW("Asset ({}) with name '{}' not found", typeid(T).name(), name)
        }
        auto asset = std::dynamic_pointer_cast<T>(m_assets[m_tracker.at(name)]);
        DE_ASSERT(asset != nullptr, "Asset is not a {}", typeid(T).name())
        return asset;
    }

    std::vector<Ref<Asset>> m_assets;
    std::unordered_map<std::string, size_t> m_tracker;
};
}  // namespace de
