#pragma once

#include "Core/pch.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"
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

        /**
         * @brief Getter for file logger.
         * @return A reference to file logger.
         */
        inline static auto& GetFileEngineLogger() { return fileEngineLogger; }

        /**
          * @brief Getter for file logger.
          * @return A reference to file logger.
          */
        inline static auto& GetFileApplicationLogger() { return fileApplicationLogger; }

    private:
        static std::shared_ptr<spdlog::logger> engineLogger;
        static std::shared_ptr<spdlog::logger> applicationLogger;
        static std::shared_ptr<spdlog::logger> fileEngineLogger;
        static std::shared_ptr<spdlog::logger> fileApplicationLogger;
    };
}  // namespace de

#ifdef NDEBUG
#define LOG_ENGINE_TRACE(...)       de::Log::GetEngineLogger()->trace(__VA_ARGS__);\
                                    de::Log::GetFileEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_DEBUG(...)       de::Log::GetEngineLogger()->debug(__VA_ARGS__);\
                                    de::Log::GetFileEngineLogger()->debug(__VA_ARGS__)
#define LOG_ENGINE_INFO(...)        de::Log::GetEngineLogger()->info(__VA_ARGS__);\
                                    de::Log::GetFileEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_WARN(...)        de::Log::GetEngineLogger()->warn(__VA_ARGS__);\
                                    de::Log::GetFileEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...)       de::Log::GetEngineLogger()->error(__VA_ARGS__);\
                                    de::Log::GetFileEngineLogger()->error(__VA_ARGS__)
#define LOG_ENGINE_CRITICAL(...)    de::Log::GetEngineLogger()->critical(__VA_ARGS__);\
                                    de::Log::GetFileEngineLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...)              de::Log::GetApplicationLogger()->trace(__VA_ARGS__);\
                                    de::Log::GetFileApplicationLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)              de::Log::GetApplicationLogger()->debug(__VA_ARGS__);\
                                    de::Log::GetFileApplicationLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)               de::Log::GetApplicationLogger()->info(__VA_ARGS__);\
                                    de::Log::GetFileApplicationLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)               de::Log::GetApplicationLogger()->warn(__VA_ARGS__);\
                                    de::Log::GetFileApplicationLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)              de::Log::GetApplicationLogger()->error(__VA_ARGS__);\
                                    de::Log::GetFileApplicationLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)           de::Log::GetApplicationLogger()->critical(__VA_ARGS__);\
                                    de::Log::GetFileApplicationLogger()->critical(__VA_ARGS__)
#else
#define LOG_ENGINE_TRACE(...)
#define LOG_ENGINE_DEBUG(...)
#define LOG_ENGINE_INFO(...)
#define LOG_ENGINE_WARN(...)
#define LOG_ENGINE_ERROR(...)
#define LOG_ENGINE_CRITICAL(...)

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#endif
