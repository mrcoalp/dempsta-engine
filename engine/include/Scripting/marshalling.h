#pragma once

#include <lua.hpp>

#include "Core/log.h"

namespace lua {
// NOTE(MPINTO): Extend _type to check for primitive
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
    static inline int GetValue(Type<int>, lua_State* L, int index) { return static_cast<int>(lua_tointeger(L, index)); }

    static inline float GetValue(Type<float>, lua_State* L, int index) {
        return static_cast<float>(lua_tonumber(L, index));
    }

    static inline double GetValue(Type<double>, lua_State* L, int index) {
        return static_cast<double>(lua_tonumber(L, index));
    }

    static inline bool GetValue(Type<bool>, lua_State* L, int index) { return lua_toboolean(L, index) != 0; }

    static inline unsigned int GetValue(Type<unsigned int>, lua_State* L, int index) {
        return static_cast<unsigned int>(lua_tointeger(L, index));
    }

    static inline std::string GetValue(Type<std::string>, lua_State* L, int index) {
        size_t size;
        const char* buffer = lua_tolstring(L, index, &size);
        return std::string{buffer, size};
    }

    static inline const char* GetValue(Type<const char*>, lua_State* L, int index) { return lua_tostring(L, index); }

    template <typename R>
    static inline R GetValue(Type<R>, lua_State* L, int index) {
        return static_cast<R>(lua_touserdata(L, index));
    }

    static void PushValue(lua_State* L, int value) { lua_pushinteger(L, value); }

    static void PushValue(lua_State* L, float value) { lua_pushnumber(L, value); }

    static void PushValue(lua_State* L, double value) { lua_pushnumber(L, value); }

    static void PushValue(lua_State* L, bool value) { lua_pushboolean(L, (int)value); }

    static void PushValue(lua_State* L, const std::string& value) { lua_pushstring(L, value.c_str()); }

    static void PushValue(lua_State* L, const char* value) { lua_pushstring(L, value); }

    static void PushValue(lua_State* L, void* value) { lua_pushlightuserdata(L, value); }

    template <typename Class>
    static void PushValue(lua_State* L, Class* value) {
        auto** a = static_cast<Class**>(lua_newuserdata(L, sizeof(Class*)));  // Create userdata
        *a = value;
        luaL_getmetatable(L, Class::Binding.GetName());
        lua_setmetatable(L, -2);
    }

    static void PushNull(lua_State* L) { lua_pushnil(L); }
};
}  // namespace lua

using MS = lua::Marshalling;
