#pragma once

#include <lua.hpp>

#include "Core/log.h"

namespace lua {
// NOTE(mpinto): Extend _type to check for primitive
template <typename T>
struct Type : std::false_type {};
template <>
struct Type<int> : std::true_type {};
template <>
struct Type<float> : std::true_type {};
template <>
struct Type<double> : std::true_type {};
template <>
struct Type<bool> : std::true_type {};
template <>
struct Type<unsigned int> : std::true_type {};
template <>
struct Type<std::string> : std::true_type {};
template <>
struct Type<const char*> : std::true_type {};

class Marshalling {
public:
    static inline int GetValue(Type<int>, lua_State* l, const int index) {
        return static_cast<int>(lua_tointeger(l, index));
    }
    static inline float GetValue(Type<float>, lua_State* l, const int index) {
        return static_cast<float>(lua_tonumber(l, index));
    }
    static inline double GetValue(Type<double>, lua_State* l, const int index) {
        return static_cast<double>(lua_tonumber(l, index));
    }
    static inline bool GetValue(Type<bool>, lua_State* l, const int index) { return lua_toboolean(l, index) != 0; }
    static inline unsigned int GetValue(Type<unsigned int>, lua_State* l, const int index) {
        return static_cast<unsigned int>(lua_tointeger(l, index));
    }
    static inline std::string GetValue(Type<std::string>, lua_State* l, const int index) {
        size_t size;
        const char* buffer = lua_tolstring(l, index, &size);
        return std::string{buffer, size};
    }
    static inline const char* GetValue(Type<const char*>, lua_State* l, const int index) {
        return lua_tostring(l, index);
    }
    template <typename R>
    static inline typename std::enable_if<!Type<R>::value, R>::type GetValue(Type<R>, lua_State* l, const int index) {
        // TODO(mpinto): Implement this
        LOG_ENGINE_ERROR("Non primitive types not yet supported in Lua marshalling!");
        return R{};
    }

    static void PushValue(lua_State* L, int value) { lua_pushinteger(L, value); }
    static void PushValue(lua_State* L, float value) { lua_pushnumber(L, value); }
    static void PushValue(lua_State* L, double value) { lua_pushnumber(L, value); }
    static void PushValue(lua_State* L, bool value) { lua_pushboolean(L, (int)value); }
    static void PushValue(lua_State* L, const std::string& value) { lua_pushstring(L, value.c_str()); }
    static void PushValue(lua_State* L, const char* value) { lua_pushstring(L, value); }
};
}  // namespace lua

using MS = lua::Marshalling;
