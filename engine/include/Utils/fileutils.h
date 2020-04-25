#pragma once

#include "Core/pch.h"

namespace de {
class FileUtils {
public:
    static std::string ReadFile(const std::string& filePath) {
        std::ifstream _if(filePath, std::ios::binary);
        std::string _result;
        if (_if.is_open()) {
            _if.seekg(0, std::ios::end);
            _result.resize(_if.tellg());
            _if.seekg(0, std::ios::beg);
            _if.read(&_result[0], _result.size());
            _if.close();
        } else {
            LOG_ENGINE_ERROR("Failed to open file: '{0}'", filePath);
            return "";
        }
        return _result;
    }
};
}  // namespace de
