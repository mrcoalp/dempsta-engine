#pragma once

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
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
        inline static auto& GetEngineLogger() { return engineLogger; }

        /**
         * @brief Getter for the application logger.
         * @return A reference to the application logger
         */
        inline static auto& GetApplicationLogger() { return applicationLogger; }

    private:
        static std::shared_ptr<spdlog::logger> engineLogger;
        static std::shared_ptr<spdlog::logger> applicationLogger;
    };
}  // namespace de

#ifdef NDEBUG
#define LOG_ENGINE_TRACE(...) de::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_INFO(...) de::Log::GetEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_WARN(...) de::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...) de::Log::GetEngineLogger()->error(__VA_ARGS__)

#define LOG_TRACE(...) de::Log::GetApplicationLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) de::Log::GetApplicationLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) de::Log::GetApplicationLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) de::Log::GetApplicationLogger()->error(__VA_ARGS__)
#else
#define LOG_ENGINE_TRACE(...)
#define LOG_ENGINE_INFO(...)
#define LOG_ENGINE_WARN(...)
#define LOG_ENGINE_ERROR(...)

#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#endif
