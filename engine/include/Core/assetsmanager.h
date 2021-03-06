#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Core/core.h"
#include "Renderer/font.h"
#include "Renderer/shader.h"
#include "Renderer/subtexture.h"
#include "Sound/sound.h"
#include "Utils/fileutils.h"

namespace de {
enum class AssetType { Undefined, Atlas, Font, Sound, Script, Shader };

class Asset {
public:
    explicit Asset(std::string filepath) : m_filepath(std::move(filepath)) {}
    virtual ~Asset() = default;

    [[nodiscard]] inline const AssetType& GetType() const { return m_type; }

    [[nodiscard]] inline const std::string& GetFilePath() const { return m_filepath; }

protected:
    AssetType m_type{AssetType::Undefined};

private:
    std::string m_filepath;
};

class AtlasAsset : public Asset {
public:
    explicit AtlasAsset(const std::string& filepath, const glm::vec2& cellSize) : Asset(filepath) {
        m_type = AssetType::Atlas;
        m_atlas = CreateRef<Atlas2D>(filepath, cellSize);
    }

    [[nodiscard]] const Ref<Atlas2D>& GetAtlas() const { return m_atlas; }

private:
    Ref<Atlas2D> m_atlas;
};

class FontAsset : public Asset {
public:
    explicit FontAsset(const std::string& filepath, Ref<Font> font) : Asset(filepath), m_font(std::move(font)) { m_type = AssetType::Font; }

    [[nodiscard]] const Ref<Font>& GetFont() const { return m_font; }

private:
    Ref<Font> m_font;
};

struct SoundAsset : public Asset {
    explicit SoundAsset(const std::string& filepath) : Asset(filepath) { m_type = AssetType::Sound; }
};

struct ScriptAsset : public Asset {
    explicit ScriptAsset(const std::string& filepath) : Asset(filepath) { m_type = AssetType::Script; }
};

class ShaderAsset : public Asset {
public:
    explicit ShaderAsset(const std::string& filepath) : Asset(filepath) {
        m_type = AssetType::Shader;
        m_shader = Shader::Create(filepath);
    }

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

    [[nodiscard]] inline const std::vector<Ref<Asset>>& GetAssets() const { return m_assets; }

    [[nodiscard]] inline const auto& GetTracker() const { return m_tracker; }

    [[nodiscard]] inline bool Exists(const std::string& name) const { return m_tracker.find(name) != m_tracker.end(); }

    [[nodiscard]] inline bool IsAtlas(const std::string& name) const {
        if (!Exists(name)) {
            return false;
        }
        return m_assets[m_tracker.at(name)]->GetType() == AssetType::Atlas;
    }

    [[nodiscard]] inline bool IsFont(const std::string& name) const {
        if (!Exists(name)) {
            return false;
        }
        return m_assets[m_tracker.at(name)]->GetType() == AssetType::Font;
    }

    [[nodiscard]] inline bool IsSound(const std::string& name) const {
        if (!Exists(name)) {
            return false;
        }
        return m_assets[m_tracker.at(name)]->GetType() == AssetType::Sound;
    }

    [[nodiscard]] inline bool IsScript(const std::string& name) const {
        if (!Exists(name)) {
            return false;
        }
        return m_assets[m_tracker.at(name)]->GetType() == AssetType::Script;
    }

    AssetsManager& AddAtlas(const std::string& name, const std::string& filepath, const glm::vec2& cellSize);

    AssetsManager& AddFont(const std::string& name, const std::string& filepath, unsigned size);

    AssetsManager& AddSound(const std::string& name, const std::string& filepath);

    AssetsManager& AddScript(const std::string& name, const std::string& filepath);

    AssetsManager& AddShader(const std::string& name, const std::string& filepath);

    [[nodiscard]] const Ref<Atlas2D>& GetAtlas(const std::string& name) const;

    [[nodiscard]] const Ref<Font>& GetFont(const std::string& name) const;

    [[nodiscard]] const std::string& GetScript(const std::string& name) const;

    [[nodiscard]] const Ref<Shader>& GetShader(const std::string& name) const;

    Ref<SubTexture2D> CreateSprite(const std::string& atlasName) const;

    Ref<SubTexture2D> CreateSprite(const std::string& atlasName, const glm::vec2& coords, const glm::vec2& spriteSize = {1.f, 1.f}) const;

    Ref<SoundInstance> CreateSoundInstance(const std::string& name) const;

private:
    AssetsManager() = default;

    [[nodiscard]] bool sourceLoaded(const std::string& filepath, size_t& index);

    AssetsManager& add(const std::string& name, const std::string& filepath, const std::function<Ref<Asset>()>& assetGetter);

    template <class T>
    Ref<T> get(const std::string& name) const {
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
