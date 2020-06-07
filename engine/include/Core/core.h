#pragma once

#include "Core/log.h"

#define DE_ASSERT(status, ...)            \
    if ((status) == 0) {                    \
        LOG_ENGINE_CRITICAL(__VA_ARGS__); \
        throw;                            \
    }

namespace de {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;
}  // namespace de
