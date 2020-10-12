#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace de {
class Math {
public:
    static constexpr float PI = 3.1415926535897932384626433832795f;
    static constexpr float PI_OVER_180 = 0.017453292519943295769236907684886f;
    static constexpr float PI_OVER_360 = 0.0087266462599716478846184538424431f;
    static constexpr float DEG2RAD = PI / 180;
    static constexpr float RAD2DEG = 180 / PI;

    /**
     * @brief Clamps value to closest min or max values in interval.
     *
     * @param value Value to clamp
     * @param minValue Min value of interval
     * @param maxValue Max value of interval
     * @return float
     */
    static inline float Clamp(float value, float minValue, float maxValue) {
        if (value < minValue) {
            return minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }

    static std::tuple<glm::vec3, glm::vec3, glm::vec3> GetTransformDecomposition(const glm::mat4& transform) {
        glm::vec3 scale;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::quat orientation;
        glm::decompose(transform, scale, orientation, translation, skew, perspective);
        glm::vec3 rotation = glm::degrees(glm::eulerAngles(orientation));
        return {translation, rotation, scale};
    }
};
}  // namespace de
