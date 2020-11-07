#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Core/core.h"
#include "Renderer/font.h"
#include "Renderer/shader.h"
#include "Renderer/subtexture.h"
#include "Utils/fileutils.h"

namespace de {
enum class AssetType { Undefined, Sprite, Font, Sound, Script, Shader };

class Asset {
public:
    explicit Asset(std::string filepath) : m_filepath(std::move(filepath)) {}
    virtual ~Asset() = default;

    [[nodiscard]] inline const AssetType& GetType() const { return m_type; }

    [[nodiscard]] inline const std::string& GetFilePath() const { return m_filepath; }

    inline void SetFilePath(const std::string& filepath) { m_filepath = filepath; }

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

class FontAsset : public Asset {
public:
    explicit FontAsset(const std::string& filepath, const Ref<Font>& font) : m_font(font), Asset(filepath) { m_type = AssetType::Font; }

    [[nodiscard]] const Ref<Font>& GetFont() const { return m_font; }

private:
    Ref<Font> m_font;
};

struct SoundAsset : public Asset {
    explicit SoundAsset(const std::string& filepath) : Asset(filepath) { m_type = AssetType::Sound; }
};

class ScriptAsset : public Asset {
public:
    explicit ScriptAsset(const std::string& filepath) : Asset(filepath) {
        m_type = AssetType::Script;
        Reload();
    }

    [[nodiscard]] inline const std::string& GetCode() const { return m_code; }

    inline void Reload() { m_code = FileUtils::ReadFile(GetFilePath()); }

private:
    std::string m_code;
};

class ShaderAsset : public Asset {
public:
    explicit ShaderAsset(const std::string& filepath, const Ref<Shader>& shader) : m_shader(shader), Asset(filepath) { m_type = AssetType::Shader; }

    [[nodiscard]] const Ref<Shader>& GetShader() const { return m_shader; }

private:
    Ref<Shader> m_shader;
};

class AssetsManager {
public:
    static AssetsManager& GetInstance();

    AssetsManager(const AssetsManager&) = delete;

    void operator=(const AssetsManager&) = delete;

    ~AssetsManager();

    void InitFreeType();

    [[nodiscard]] inline bool Exists(const std::string& name) const { return m_tracker.find(name) != m_tracker.end(); }

    [[nodiscard]] inline const std::string& GetFilePath(const std::string& name) const {
        DE_ASSERT(Exists(name), "Asset '{}' does not exist", name)
        return m_assets[m_tracker.at(name)]->GetFilePath();
    }

    inline void SetFilePath(const std::string& name, const std::string& filepath) {
        DE_ASSERT(Exists(name), "Asset '{}' does not exist", name)
        m_assets[m_tracker.at(name)]->SetFilePath(filepath);
    }

    [[nodiscard]] inline bool IsSprite(const std::string& name) const { return m_assets[m_tracker.at(name)]->GetType() == AssetType::Sprite; }

    [[nodiscard]] inline bool IsFont(const std::string& name) const { return m_assets[m_tracker.at(name)]->GetType() == AssetType::Font; }

    [[nodiscard]] inline bool IsSound(const std::string& name) const { return m_assets[m_tracker.at(name)]->GetType() == AssetType::Sound; }

    [[nodiscard]] inline bool IsScript(const std::string& name) const { return m_assets[m_tracker.at(name)]->GetType() == AssetType::Script; }

    AssetsManager& AddSprite(const std::string& name, const std::string& filepath);

    AssetsManager& AddFont(const std::string& name, const std::string& filepath, unsigned size);

    AssetsManager& AddScript(const std::string& name, const std::string& filepath);

    AssetsManager& AddShader(const std::string& name, const std::string& filepath);

    [[nodiscard]] const Ref<SubTexture2D>& GetSprite(const std::string& name) const;

    [[nodiscard]] const Ref<Font>& GetFont(const std::string& name) const;

    [[nodiscard]] const Ref<ScriptAsset>& GetScript(const std::string& name) const;

    [[nodiscard]] const Ref<Shader>& GetShader(const std::string& name) const;

private:
    AssetsManager() = default;

    [[nodiscard]] bool sourceLoaded(const std::string& filepath, size_t& index);

    AssetsManager& add(const std::string& name, const std::string& filepath, const std::function<Ref<Asset>()>& assetGetter);

    template <class T>
    const Ref<T>& get(const std::string& name) const {
        if (!Exists(name)) {
            DE_THROW("Asset ({}) with name '{}' not found", typeid(T).name(), name)
        }
        auto asset = std::dynamic_pointer_cast<T>(m_assets[m_tracker.at(name)]);
        DE_ASSERT(asset != nullptr, "Asset is not a {}", typeid(T).name())
        return asset;
    }

    FT_Library m_fontLibrary;
    FT_Long m_fontIndex{0};
    std::vector<Ref<Asset>> m_assets;
    std::unordered_map<std::string, size_t> m_tracker;
};
}  // namespace de
