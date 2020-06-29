#pragma once

#include <lua.hpp>
#include <string>

#include "Scripting/luaclass.h"
#include "Scripting/marshalling.h"

namespace lua {
typedef int (*lua_CFunction)(lua_State* L);

class ScriptManager {
public:
    static void Init();

    static void CloseState();

    static bool LoadFile(const std::string& filePath);

    [[nodiscard]] static inline const lua_State* GetState() { return state; }

    template <class T>
    static void RegisterClass(const char* nameSpace = nullptr) {
        LuaClass<T>::Register(state, nameSpace);
    }

    static void RegisterFunction(const char* name, lua_CFunction fn);

    template <typename... Args>
    static void CallFunction(const char* name, Args... args) {
        lua_getglobal(state, name);
        if (!lua_isfunction(state, -1)) {
            LOG_ENGINE_WARN("Tried to call an invalid Lua function: {}", name);
            lua_pop(state, 1);
            return;
        }
        PushValues(std::forward<Args>(args)...);
        lua_call(state, sizeof...(Args), 0);
    }

    template <typename T, typename... Args>
    static void CallFunction(T& rValue, const char* name, Args... args) {
        lua_getglobal(state, name);
        if (!lua_isfunction(state, -1)) {
            LOG_ENGINE_WARN("Tried to call an invalid Lua function: {}", name);
            lua_pop(state, 1);
            return;
        }
        PushValues(std::forward<Args>(args)...);
        lua_call(state, sizeof...(Args), 1);
        rValue = GetValue<T>(-1);
    }

    static void CallFunction(const char* name);

    template <typename R>
    [[nodiscard]] static inline R GetValue(const int index = 1) {
        return MS::GetValue(lua::Type<R>{}, state, index);
    }

    template <typename T>
    static void PushValue(T value) {
        MS::PushValue(state, value);
    }

    template <typename T, typename... Args>
    static void PushValues(T first, Args... args) {
        PushValue(first);
        PushValues(std::forward<Args>(args)...);
    }

    template <typename T>
    static void PushValues(T first) {
        PushValue(first);
    }

private:
    static lua_State* state;
};
}  // namespace lua

using SM = lua::ScriptManager;
