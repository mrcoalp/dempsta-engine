#pragma once

#include "Renderer/font.h"

namespace de {
class Label {
public:
    Label(const Ref<Font>& font, std::string content) : m_font(font), m_content(std::move(content)) {}

    [[nodiscard]] inline const Ref<Font>& GetFont() const { return m_font; }

    [[nodiscard]] inline const std::string& GetContent() const { return m_content; }

    void SetContent(const std::string& content) { m_content = content; }

private:
    const Ref<Font>& m_font;
    std::string m_fontName;
    std::string m_content;
};
}  // namespace de
