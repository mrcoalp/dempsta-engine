#pragma once

#include "Core/log.h"

#define DE_ASSERT(status, ...)            \
    if (status == 0) {                    \
        LOG_ENGINE_CRITICAL(__VA_ARGS__); \
        throw;                            \
    }
