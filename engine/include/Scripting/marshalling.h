#pragma once

#include <lua.hpp>

namespace lua {
template <typename T>
struct Type {
    static constexpr bool isPrimitive = false;
};
template <>
struct Type<int> {
    static constexpr bool isPrimitive = true;
};
template <>
struct Type<float> {
    static constexpr bool isPrimitive = true;
};
template <>
struct Type<double> {
    static constexpr bool isPrimitive = true;
};
template <>
struct Type<bool> {
    static constexpr bool isPrimitive = true;
};
template <>
struct Type<unsigned int> {
    static constexpr bool isPrimitive = true;
};
template <>
struct Type<std::string> {
    static constexpr bool isPrimitive = true;
};

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

    static void PushValue(lua_State* L, int value) { lua_pushinteger(L, value); }
    static void PushValue(lua_State* L, float value) { lua_pushnumber(L, value); }
    static void PushValue(lua_State* L, double value) { lua_pushnumber(L, value); }
    static void PushValue(lua_State* L, bool value) { lua_pushboolean(L, value); }
    static void PushValue(lua_State* L, const std::string& value) { lua_pushstring(L, value.c_str()); }
    static void PushValue(lua_State* L, const char* value) { lua_pushstring(L, value); }
};
}  // namespace lua

using MS = lua::Marshalling;
