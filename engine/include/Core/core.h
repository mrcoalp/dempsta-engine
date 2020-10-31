#pragma once

#include "Core/log.h"

/** Runtime Dempsta assert. */
#define DE_ASSERT(status, ...)            \
    if (!(status)) {                        \
        LOG_ENGINE_CRITICAL(__VA_ARGS__); \
        throw;                            \
    }

#define DE_THROW(...)                 \
    LOG_ENGINE_CRITICAL(__VA_ARGS__); \
    throw;

namespace de {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}  // namespace de
