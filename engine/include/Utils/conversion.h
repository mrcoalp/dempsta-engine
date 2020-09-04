#pragma once

#include <sstream>

namespace de {
class Conversion {
public:
    template <typename T>
    static T HexStringToHexValue(const std::string& string, bool head = false) {
        std::stringstream ss(head ? string.substr(2) : string);
        T hex;
        ss >> std::hex >> hex;
        return hex;
    }

    template <typename T>
    static std::string HexValueToHexString(const T& value) {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(T) * 2) << value;
        return ss.str();
    }
};
}
