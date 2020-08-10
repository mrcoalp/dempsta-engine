#include "Scripting/API/databuffer.h"

namespace lua {
LUA_DEFINE_BINDING(DataBuffer, false)
LUA_ADD_METHOD(Set)
LUA_ADD_METHOD(Get);
}  // namespace lua
