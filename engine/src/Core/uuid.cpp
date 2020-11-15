#include "Core/uuid.h"

#include <random>
#include <sstream>

namespace de {
static std::random_device randomDevice;
static std::mt19937_64 generator(randomDevice());
static std::uniform_int_distribution<uint64_t> distribution;

UUID::UUID() : m_uuid(distribution(generator)) {}
}  // namespace de
