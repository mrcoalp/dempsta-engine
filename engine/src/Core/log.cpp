#include "Core/log.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace de {
std::shared_ptr<spdlog::logger> Log::engineLogger;
std::shared_ptr<spdlog::logger> Log::applicationLogger;
std::shared_ptr<spdlog::logger> Log::fileEngineLogger;
std::shared_ptr<spdlog::logger> Log::fileApplicationLogger;

void Log::Init() {
    engineLogger = spdlog::stdout_color_mt("DEMPSTA");
    engineLogger->set_level(spdlog::level::trace);
    engineLogger->set_pattern("%^[%t][%H:%M:%S.%e][ENGINE]: %v%$");

    applicationLogger = spdlog::stdout_color_mt("APP");
    applicationLogger->set_level(spdlog::level::trace);
    applicationLogger->set_pattern("%^[%t][%H:%M:%S.%e]: %v%$");

    fileEngineLogger = spdlog::daily_logger_mt<spdlog::async_factory>("DEMPSTA_", "logs/dempsta.log");
    fileEngineLogger->set_level(spdlog::level::trace);
    fileEngineLogger->set_pattern("%^[%t][%H:%M:%S.%e][ENGINE]: %v%$");

    fileApplicationLogger = spdlog::daily_logger_mt<spdlog::async_factory>("APP_", "logs/dempsta.log");
    fileApplicationLogger->set_level(spdlog::level::trace);
    fileApplicationLogger->set_pattern("%^[%t][%H:%M:%S.%e]: %v%$");

    spdlog::flush_every(std::chrono::seconds(1));
}
}  // namespace de
