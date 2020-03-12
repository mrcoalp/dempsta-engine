#include "Core/log.h"

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"

namespace de {
    std::shared_ptr<spdlog::logger> Log::engineLogger;
    std::shared_ptr<spdlog::logger> Log::applicationLogger;
    std::shared_ptr<spdlog::logger> Log::fileEngineLogger;
    std::shared_ptr<spdlog::logger> Log::fileApplicationLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%t][%H:%M:%S.%e][%n]: %v%$");
        engineLogger = spdlog::stdout_color_mt("DEMPSTA");
        engineLogger->set_level(spdlog::level::trace);

        applicationLogger = spdlog::stdout_color_mt("APP");
        applicationLogger->set_level(spdlog::level::trace);

        fileEngineLogger = spdlog::daily_logger_mt<spdlog::async_factory>(
                "_DEMPSTA_",
                "logs/dempsta.log"
        );
        fileEngineLogger->set_level(spdlog::level::trace);

        fileApplicationLogger = spdlog::daily_logger_mt<spdlog::async_factory>(
                "_APP_",
                "logs/dempsta.log"
        );
        fileApplicationLogger->set_level(spdlog::level::trace);

        spdlog::flush_every(std::chrono::seconds(1));
    }
}  // namespace de
