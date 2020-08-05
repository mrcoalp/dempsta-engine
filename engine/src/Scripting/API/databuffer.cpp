#include "Scripting/API/databuffer.h"

namespace lua {
LUA_DEFINE_BINDING(DataBuffer, false)
LUA_ADD_METHOD(SetInt)
LUA_ADD_METHOD(GetInt)
LUA_ADD_METHOD(SetFloat)
LUA_ADD_METHOD(GetFloat)
LUA_ADD_METHOD(SetDouble)
LUA_ADD_METHOD(GetDouble)
LUA_ADD_METHOD(SetBool)
LUA_ADD_METHOD(GetBool)
LUA_ADD_METHOD(SetString)
LUA_ADD_METHOD(GetString);
}  // namespace lua
