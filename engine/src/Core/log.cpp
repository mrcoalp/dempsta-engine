#include "Core/log.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace de {
std::shared_ptr<spdlog::logger> Log::m_engineLogger;
std::shared_ptr<spdlog::logger> Log::m_applicationLogger;
std::shared_ptr<spdlog::logger> Log::m_fileEngineLogger;
std::shared_ptr<spdlog::logger> Log::m_fileApplicationLogger;

void Log::Init() {
    m_engineLogger = spdlog::stdout_color_mt("DEMPSTA");
    m_engineLogger->set_level(spdlog::level::trace);
    m_engineLogger->set_pattern("%^[%t][%H:%M:%S.%e][ENGINE]: %v%$");

    m_applicationLogger = spdlog::stdout_color_mt("APP");
    m_applicationLogger->set_level(spdlog::level::trace);
    m_applicationLogger->set_pattern("%^[%t][%H:%M:%S.%e]: %v%$");

    m_fileEngineLogger = spdlog::daily_logger_mt<spdlog::async_factory>("DEMPSTA_", "logs/dempsta.log");
    m_fileEngineLogger->set_level(spdlog::level::trace);
    m_fileEngineLogger->set_pattern("%^[%t][%H:%M:%S.%e][ENGINE]: %v%$");

    m_fileApplicationLogger = spdlog::daily_logger_mt<spdlog::async_factory>("APP_", "logs/dempsta.log");
    m_fileApplicationLogger->set_level(spdlog::level::trace);
    m_fileApplicationLogger->set_pattern("%^[%t][%H:%M:%S.%e]: %v%$");

    spdlog::flush_every(std::chrono::seconds(1));
}
}  // namespace de
