#pragma once

#include <lua.hpp>
#include <map>
#include <stdexcept>

namespace lua {
template <typename T>
using LuaMap = std::unordered_map<std::string, T>;

template <typename T>
struct Type {};

class Marshalling {
public:
    static inline int GetValue(Type<int>, lua_State* L, int index) {
        ensure_type(lua_isinteger(L, index));
        return static_cast<int>(lua_tointeger(L, index));
    }

    static inline float GetValue(Type<float>, lua_State* L, int index) {
        ensure_type(lua_isnumber(L, index));
        return static_cast<float>(lua_tonumber(L, index));
    }

    static inline double GetValue(Type<double>, lua_State* L, int index) {
        ensure_type(lua_isnumber(L, index));
        return static_cast<double>(lua_tonumber(L, index));
    }

    static inline bool GetValue(Type<bool>, lua_State* L, int index) {
        ensure_type(lua_isboolean(L, index));
        return lua_toboolean(L, index) != 0;
    }

    static inline unsigned GetValue(Type<unsigned>, lua_State* L, int index) {
        ensure_type(lua_isinteger(L, index));
        return static_cast<unsigned>(lua_tointeger(L, index));
    }

    static inline std::string GetValue(Type<std::string>, lua_State* L, int index) {
        ensure_type(lua_isstring(L, index));
        size_t size;
        const char* buffer = lua_tolstring(L, index, &size);
        return std::string{buffer, size};
    }

    static inline const char* GetValue(Type<const char*>, lua_State* L, int index) {
        ensure_type(lua_isstring(L, index));
        return lua_tostring(L, index);
    }

    static inline void* GetValue(Type<void*>, lua_State* L, int index) {
        ensure_type(lua_isuserdata(L, index));
        return lua_touserdata(L, index);
    }

    template <typename R>
    static inline R GetValue(Type<R>, lua_State* L, int index) {
        ensure_type(lua_isuserdata(L, index));
        return static_cast<R>(lua_touserdata(L, index));
    }

    template <typename T>
    static std::vector<T> GetValue(Type<std::vector<T>>, lua_State* L, int index) {
        ensure_type(lua_istable(L, index));
        size_t size = lua_rawlen(L, -1);
        std::vector<T> vec;
        vec.reserve(size);
        for (size_t i = 1; i <= size; ++i) {
            lua_pushinteger(L, i);
            lua_gettable(L, -2);
            if (lua_type(L, -1) == LUA_TNIL) {
                break;
            }
            vec.emplace_back(GetValue(Type<T>{}, L, -1));
            lua_pop(L, 1);
        }
        return vec;
    }

    template <typename T>
    [[nodiscard]] static LuaMap<T> GetValue(Type<LuaMap<T>>, lua_State* L, int index) {
        ensure_type(lua_istable(L, index));
        lua_pushnil(L);
        LuaMap<T> map;
        while (lua_next(L, -2) != 0) {
            map.emplace(GetValue(Type<std::string>{}, L, -2), GetValue(Type<T>{}, L, -1));
            lua_pop(L, 1);
        }
        return map;
    }

    static void PushValue(lua_State* L, int value) { lua_pushinteger(L, value); }

    static void PushValue(lua_State* L, float value) { lua_pushnumber(L, value); }

    static void PushValue(lua_State* L, double value) { lua_pushnumber(L, value); }

    static void PushValue(lua_State* L, bool value) { lua_pushboolean(L, (int)value); }

    static void PushValue(lua_State* L, unsigned value) { lua_pushinteger(L, value); }

    static void PushValue(lua_State* L, const std::string& value) { lua_pushstring(L, value.c_str()); }

    static void PushValue(lua_State* L, const char* value) { lua_pushstring(L, value); }

    static void PushValue(lua_State* L, void* value) { lua_pushlightuserdata(L, value); }

    template <typename T>
    static void PushValue(lua_State* L, const std::vector<T>& value) {
        lua_newtable(L);
        unsigned index = 1;
        for (const auto& element : value) {
            PushValue(L, index);
            PushValue(L, element);
            lua_settable(L, -3);
            ++index;
        }
    }

    template <typename T>
    static void PushValue(lua_State* L, const std::unordered_map<std::string, T>& value) {
        lua_newtable(L);
        for (const auto& element : value) {
            PushValue(L, element.first);
            PushValue(L, element.second);
            lua_settable(L, -3);
        }
    }

    template <typename T>
    static void PushValue(lua_State* L, const std::map<std::string, T>& value) {
        lua_newtable(L);
        for (const auto& element : value) {
            PushValue(L, element.first);
            PushValue(L, element.second);
            lua_settable(L, -3);
        }
    }

    template <typename Class>
    static void PushValue(lua_State* L, Class* value) {
        auto** a = static_cast<Class**>(lua_newuserdata(L, sizeof(Class*)));  // Create userdata
        *a = value;
        luaL_getmetatable(L, Class::Binding.GetName());
        lua_setmetatable(L, -2);
    }

    static void PushNull(lua_State* L) { lua_pushnil(L); }

private:
    static void ensure_type(int check) {
        if (check != 1) {
            throw std::runtime_error("Lua type check failed!");
        }
    }
};
}  // namespace lua

using MS = lua::Marshalling;
