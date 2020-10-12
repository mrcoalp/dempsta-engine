#pragma once

#include "Renderer/font.h"

namespace de {
class Label {
public:
    Label(std::string fontName, std::string content) : m_fontName(std::move(fontName)), m_content(std::move(content)) {}

    [[nodiscard]] inline Font GetFont() const { return FontManager::GetInstance().GetFont(m_fontName); }

    [[nodiscard]] inline const std::string& GetContent() const { return m_content; }

    void SetContent(const std::string& content) { m_content = content; }

private:
    std::string m_fontName;
    std::string m_content;
};
}  // namespace de