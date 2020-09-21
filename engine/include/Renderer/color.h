#pragma once

#include <glm/glm.hpp>
#include <iomanip>

#include "Utils/conversion.h"

namespace de {
class Color {
public:
    static glm::vec3 HexStringToVec3(const std::string& hexColorString) {
        auto tmp = hexColorString;
        if (tmp.substr(0, 1) == "#") {
            tmp.erase(0, 1);
        } else if (tmp.substr(0, 2) == "0x") {
            tmp.erase(0, 2);
        }
        if (tmp.size() != 6) {
            LOG_ENGINE_WARN("Tried to convert an invalid hex string to vec3: '{}'", hexColorString);
            return glm::vec3();
        }
        auto hexColor = Conversion::HexStringToHexValue<uint32_t>(tmp);
        glm::vec3 newColor;
        newColor.r = static_cast<float>((hexColor >> 16u) & 0xFF) / 255.f;
        newColor.g = static_cast<float>((hexColor >> 8u) & 0xFF) / 255.f;
        newColor.b = static_cast<float>(hexColor & 0xFF) / 255.f;
        return newColor;
    }

    static glm::vec4 HexStringToVec4(const std::string& hexColorString) {
        auto tmp = hexColorString;
        if (tmp.substr(0, 1) == "#") {
            tmp.erase(0, 1);
        } else if (tmp.substr(0, 2) == "0x") {
            tmp.erase(0, 2);
        }
        if (tmp.size() != 8 && tmp.size() != 6) {
            LOG_ENGINE_WARN("Tried to convert an invalid hex string to vec4: '{}'", hexColorString);
            return glm::vec4();
        }
        if (tmp.size() == 6) {
            return {HexStringToVec3(tmp), 1.f};
        }
        auto hexColor = Conversion::HexStringToHexValue<uint32_t>(tmp);
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
        return Conversion::HexValueToHexString(hexColor);
    }

    static std::string Vec4ToHexString(const glm::vec4& color) {
        auto r = static_cast<uint32_t>(color.r * 255.f);
        auto g = static_cast<uint32_t>(color.g * 255.f);
        auto b = static_cast<uint32_t>(color.b * 255.f);
        auto a = static_cast<uint32_t>(color.a * 255.f);
        uint32_t hexColor = r << 24u | g << 16u | b << 8u | a;
        return Conversion::HexValueToHexString(hexColor);
    }
};
}  // namespace de
