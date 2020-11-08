#include "Utils/fileutils.h"

#include "Core/log.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <GLFW/glfw3.h>
#include <commdlg.h>

#include "Core/application.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace de {
std::string FileUtils::ReadFile(const std::string& filePath) {
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

void FileUtils::WriteFile(const std::string& filePath, const std::string& data, bool append) {
    std::ofstream out;
    append ? out.open(filePath, std::ios_base::app) : out.open(filePath);
    if (out.is_open()) {
        out << data;
        out.close();
    } else {
        LOG_ENGINE_ERROR("Failed to create/write file: '{0}'", filePath);
    }
}

std::string FileUtils::GetFileNameWithExtension(std::string filepath) {
    // Remove directory if present.
    const size_t lastSlashIndex = filepath.find_last_of("\\/");
    if (std::string::npos != lastSlashIndex) {
        filepath.erase(0, lastSlashIndex + 1);
    }
    return filepath;
}

std::string FileUtils::GetFileName(const std::string& filepath) {
    std::string fileNameWithExtension = GetFileNameWithExtension(filepath);
    // Remove extension if present.
    const size_t periodIndex = fileNameWithExtension.rfind('.');
    if (std::string::npos != periodIndex) {
        fileNameWithExtension.erase(periodIndex);
    }
    return fileNameWithExtension;
}

std::string FileUtils::GetFileExtension(const std::string& filepath) {
    std::string fileNameWithExtension = GetFileNameWithExtension(filepath);
    // Remove filename.
    const size_t periodIndex = fileNameWithExtension.rfind('.');
    if (std::string::npos != periodIndex) {
        fileNameWithExtension.erase(0, periodIndex + 1);
    }
    return fileNameWithExtension;
}

std::optional<std::string> FileUtils::OpenFile(const char* filter) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }
    return std::nullopt;

#elif defined(__linux) || defined(__linux__) || defined(linux)

    char filepath[1024];
    char command[1024];
    sprintf(command, "zenity --file-selection --file-filter='%s' --modal --title=\"%s\" ", filter, "Select file to open");
    FILE* file = popen(command, "r");
    memset(filepath, 0, 1024);
    fgets(filepath, 1024, file);
    if (strlen(filepath) > 0) {
        auto path = std::string(filepath);
        path.erase(path.length() - 1);
        return path;
    }
    return std::nullopt;

#else

    return std::nullopt;

#endif
}

std::optional<std::string> FileUtils::SaveFile(const char* filter) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Sets the default extension by extracting it from the filter
    ofn.lpstrDefExt = strchr(filter, '\0') + 1;

    if (GetSaveFileNameA(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }
    return std::nullopt;

#elif defined(__linux) || defined(__linux__) || defined(linux)

    char filepath[1024];
    char command[1024];
    sprintf(command, "zenity --file-selection --save --file-filter='%s' --modal --title=\"%s\" ", filter, "Select file to save");
    FILE* file = popen(command, "r");
    memset(filepath, 0, 1024);
    fgets(filepath, 1024, file);
    if (strlen(filepath) > 0) {
        auto path = std::string(filepath);
        path.erase(path.length() - 1);
        return path;
    }
    return std::nullopt;

#else

    return std::nullopt;

#endif
}
}  // namespace de
