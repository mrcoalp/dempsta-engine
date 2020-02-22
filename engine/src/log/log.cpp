#include "log.h"

namespace de {

    std::shared_ptr <spdlog::logger> Log::engineLogger;
    std::shared_ptr <spdlog::logger> Log::applicationLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%c] %n: %v%$");
        engineLogger = spdlog::stdout_color_mt("DEMPSTA");
        engineLogger->set_level(spdlog::level::trace);

        applicationLogger = spdlog::stdout_color_mt("APP");
        applicationLogger->set_level(spdlog::level::trace);
    }
}  // namespace de
