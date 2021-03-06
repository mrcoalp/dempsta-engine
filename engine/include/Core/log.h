#pragma once

#include "spdlog/spdlog.h"

namespace de {
class Log {
public:
    /**
     * @brief Initializes log system.
     */
    static void Init();

    /**
     * @brief Getter for the engine logger.
     * @return A reference to the engine logger
     */
    inline static auto& GetEngineLogger() { return m_engineLogger; }

    /**
     * @brief Getter for the application logger.
     * @return A reference to the application logger
     */
    inline static auto& GetApplicationLogger() { return m_applicationLogger; }

    /**
     * @brief Getter for file logger.
     * @return A reference to file logger.
     */
    inline static auto& GetFileEngineLogger() { return m_fileEngineLogger; }

    /**
     * @brief Getter for file logger.
     * @return A reference to file logger.
     */
    inline static auto& GetFileApplicationLogger() { return m_fileApplicationLogger; }

private:
    static std::shared_ptr<spdlog::logger> m_engineLogger;
    static std::shared_ptr<spdlog::logger> m_applicationLogger;
    static std::shared_ptr<spdlog::logger> m_fileEngineLogger;
    static std::shared_ptr<spdlog::logger> m_fileApplicationLogger;
};
}  // namespace de

#ifdef NDEBUG
#define LOG_ENGINE_TRACE(...)                         \
    ::de::Log::GetEngineLogger()->trace(__VA_ARGS__); \
    ::de::Log::GetFileEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_DEBUG(...)                         \
    ::de::Log::GetEngineLogger()->debug(__VA_ARGS__); \
    ::de::Log::GetFileEngineLogger()->debug(__VA_ARGS__)
#define LOG_ENGINE_INFO(...)                         \
    ::de::Log::GetEngineLogger()->info(__VA_ARGS__); \
    ::de::Log::GetFileEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_WARN(...)                         \
    ::de::Log::GetEngineLogger()->warn(__VA_ARGS__); \
    ::de::Log::GetFileEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...)                         \
    ::de::Log::GetEngineLogger()->error(__VA_ARGS__); \
    ::de::Log::GetFileEngineLogger()->error(__VA_ARGS__)
#define LOG_ENGINE_CRITICAL(...)                         \
    ::de::Log::GetEngineLogger()->critical(__VA_ARGS__); \
    ::de::Log::GetFileEngineLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...)                                     \
    ::de::Log::GetApplicationLogger()->trace(__VA_ARGS__); \
    ::de::Log::GetFileApplicationLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)                                     \
    ::de::Log::GetApplicationLogger()->debug(__VA_ARGS__); \
    ::de::Log::GetFileApplicationLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)                                     \
    ::de::Log::GetApplicationLogger()->info(__VA_ARGS__); \
    ::de::Log::GetFileApplicationLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)                                     \
    ::de::Log::GetApplicationLogger()->warn(__VA_ARGS__); \
    ::de::Log::GetFileApplicationLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)                                     \
    ::de::Log::GetApplicationLogger()->error(__VA_ARGS__); \
    ::de::Log::GetFileApplicationLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)                                     \
    ::de::Log::GetApplicationLogger()->critical(__VA_ARGS__); \
    ::de::Log::GetFileApplicationLogger()->critical(__VA_ARGS__)
#else
#define LOG_ENGINE_TRACE(...)
#define LOG_ENGINE_DEBUG(...)
#define LOG_ENGINE_INFO(...)
#define LOG_ENGINE_WARN(...) ::de::Log::GetFileEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...) ::de::Log::GetFileEngineLogger()->error(__VA_ARGS__)
#define LOG_ENGINE_CRITICAL(...) ::de::Log::GetFileEngineLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...) ::de::Log::GetFileApplicationLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::de::Log::GetFileApplicationLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::de::Log::GetFileApplicationLogger()->critical(__VA_ARGS__)
#endif
