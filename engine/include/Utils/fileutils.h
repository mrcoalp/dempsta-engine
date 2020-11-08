#pragma once

#include <fstream>
#include <optional>

namespace de {
class FileUtils {
public:
    static std::string ReadFile(const std::string& filePath);

    static void WriteFile(const std::string& filePath, const std::string& data, bool append = false);

    static std::optional<std::string> OpenFile(const char* filter);

    static std::optional<std::string> SaveFile(const char* filter);

    static std::string GetFileNameWithExtension(std::string filepath);

    static std::string GetFileName(const std::string& filepath);

    static std::string GetFileExtension(const std::string& filepath);
};
}  // namespace de
