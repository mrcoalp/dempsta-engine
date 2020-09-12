#pragma once

#include "Renderer/font.h"

namespace de {
class Label {
public:
    Label(const std::string& fontName, const std::string& content) : m_fontName(fontName), m_content(content) {}

    [[nodiscard]] inline const Font GetFont() const { return FontManager::GetInstance().GetFont(m_fontName); }

    [[nodiscard]] inline const std::string& GetContent() const { return m_content; }

    void SetContent(const std::string& content) { m_content = content; }

private:
    // TODO(mpinto): Change this when a proper font manager is implemented
    std::string m_fontName;
    std::string m_content{""};
};
}  // namespace de
