#pragma once

#include "Core/pch.h"

namespace de {
class FileUtils {
public:
    static std::string ReadFile(const std::string& filePath) {
        std::ifstream _if(filePath);
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

    static std::string GetFileNameWithExtension(std::string filepath) {
        // Remove directory if present.
        const size_t _lastSlashIndex = filepath.find_last_of("\\/");
        if (std::string::npos != _lastSlashIndex) {
            filepath.erase(0, _lastSlashIndex + 1);
        }
        return filepath;
    }

    static std::string GetFileName(const std::string& filepath) {
        std::string _fileNameWithExtension = GetFileNameWithExtension(filepath);
        // Remove extension if present.
        const size_t _periodIndex = _fileNameWithExtension.rfind('.');
        if (std::string::npos != _periodIndex) {
            _fileNameWithExtension.erase(_periodIndex);
        }
        return _fileNameWithExtension;
    }

    static std::string GetFileExtension(const std::string& filepath) {
        std::string _fileNameWithExtension = GetFileNameWithExtension(filepath);
        // Remove filename.
        const size_t _periodIndex = _fileNameWithExtension.rfind('.');
        if (std::string::npos != _periodIndex) {
            _fileNameWithExtension.erase(0, _periodIndex + 1);
        }
        return _fileNameWithExtension;
    }
};
}  // namespace de
