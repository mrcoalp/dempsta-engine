#pragma once

#include <fstream>

namespace de {
class FileUtils {
public:
    static std::string ReadFile(const std::string& filePath) {
        std::ifstream ifStream(filePath);
        std::string result;
        if (ifStream.is_open()) {
            ifStream.seekg(0, std::ios::end);
            result.resize(static_cast<unsigned int>(ifStream.tellg()));
            ifStream.seekg(0, std::ios::beg);
            ifStream.read(&result[0], result.size());
            ifStream.close();
        } else {
            LOG_ENGINE_ERROR("Failed to open file: '{0}'", filePath);
            return "";
        }
        return result;
    }

    static std::string GetFileNameWithExtension(std::string filepath) {
        // Remove directory if present.
        const size_t lastSlashIndex = filepath.find_last_of("\\/");
        if (std::string::npos != lastSlashIndex) {
            filepath.erase(0, lastSlashIndex + 1);
        }
        return filepath;
    }

    static std::string GetFileName(const std::string& filepath) {
        std::string fileNameWithExtension = GetFileNameWithExtension(filepath);
        // Remove extension if present.
        const size_t periodIndex = fileNameWithExtension.rfind('.');
        if (std::string::npos != periodIndex) {
            fileNameWithExtension.erase(periodIndex);
        }
        return fileNameWithExtension;
    }

    static std::string GetFileExtension(const std::string& filepath) {
        std::string fileNameWithExtension = GetFileNameWithExtension(filepath);
        // Remove filename.
        const size_t periodIndex = fileNameWithExtension.rfind('.');
        if (std::string::npos != periodIndex) {
            fileNameWithExtension.erase(0, periodIndex + 1);
        }
        return fileNameWithExtension;
    }
};
}  // namespace de
