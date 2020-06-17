#pragma once

#include <lua.hpp>
#include <string>

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

    template <typename R>
    [[nodiscard]] static inline R GetValue(const int index = 1) {
        return Lua::GetValue(Lua::Type<R>{}, state, index);
    }

    template <typename R>
    static void PushValue(R value) {
        Lua::PushValue(state, value);
    }

private:
    static lua_State* state;
};
}  // namespace lua

using SM = lua::ScriptManager;
