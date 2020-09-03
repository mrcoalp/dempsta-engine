#pragma once

#include <glm/glm.hpp>
#include <iomanip>

#include "Utils/stringutils.h"

namespace de {
class Color {
public:
    static glm::vec3 HexStringToVec3(const std::string& hexColorString) {
        auto hexColor = StringUtils::ToHex<uint32_t>(hexColorString);
        glm::vec3 newColor;
        newColor.r = static_cast<float>((hexColor >> 16u) & 0xFF) / 255.f;
        newColor.g = static_cast<float>((hexColor >> 8u) & 0xFF) / 255.f;
        newColor.b = static_cast<float>(hexColor & 0xFF) / 255.f;
        return newColor;
    }

    static glm::vec4 HexStringToVec4(const std::string& hexColorString) {
        auto hexColor = StringUtils::ToHex<uint32_t>(hexColorString);
        glm::vec4 newColor;
        newColor.r = static_cast<float>((hexColor >> 24u) & 0xFF) / 255.f;
        newColor.g = static_cast<float>((hexColor >> 16u) & 0xFF) / 255.f;
        newColor.b = static_cast<float>((hexColor >> 8u) & 0xFF) / 255.f;
        newColor.a = static_cast<float>(hexColor & 0xFF) / 255.f;
        return newColor;
    }

    static std::string Vec3ToHexString(const glm::vec3& color) {
        auto r = static_cast<uint32_t>(color.r * 255.f);
        auto g = static_cast<uint32_t>(color.g * 255.f);
        auto b = static_cast<uint32_t>(color.b * 255.f);
        uint32_t hexColor = r << 16u | g << 8u | b;
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(6) << hexColor;
        return ss.str();
    }

    static std::string Vec4ToHexString(const glm::vec4& color) {
        auto r = static_cast<uint32_t>(color.r * 255.f);
        auto g = static_cast<uint32_t>(color.g * 255.f);
        auto b = static_cast<uint32_t>(color.b * 255.f);
        auto a = static_cast<uint32_t>(color.a * 255.f);
        uint32_t hexColor = r << 24u | g << 16u | b << 8u | a;
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(8) << hexColor;
        return ss.str();
    }
};
}  // namespace de
