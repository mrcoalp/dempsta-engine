#pragma once

#include <sstream>

namespace de {
class StringUtils {
public:
    static std::string SubString(const std::string& string, size_t begin = 0, long end = -1) {
        end = end < 0 ? string.size() : end;
        return string.substr(begin, end);
    }
};
}  // namespace de
