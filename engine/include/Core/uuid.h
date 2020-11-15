#pragma once

#include <string>

namespace de {
class UUID {
public:
    UUID();

    explicit UUID(uint64_t uuid) : m_uuid(uuid) {}

    UUID(const UUID&) = default;

    explicit operator uint64_t() const noexcept { return m_uuid; }

private:
    uint64_t m_uuid;
};
}  // namespace de
