#pragma once

#include <sstream>

namespace de {
class StringUtils {
public:
    template <typename T>
    static T ToHex(const std::string& string, bool head = false) {
        std::stringstream ss(head ? string.substr(2) : string);
        T hex;
        ss >> std::hex >> hex;
        return hex;
    }
};
}  // namespace de
