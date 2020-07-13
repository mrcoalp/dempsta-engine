#pragma once

#include <lua.hpp>
#include <string>

#include "Scripting/luaclass.h"
#include "Scripting/marshalling.h"

namespace lua {
using lua_CFunction = int (*)(lua_State*);

/**
 * @brief Handles all the logic related to "communication" between C++ and Lua, initializing it.
 * Acts as an engine to allow to call C++ functions from Lua and vice-versa, registers
 * and exposes C++ classes to Lua, pops and pushes values to Lua stack, runs file scripts and
 * string scripts.
 */
class ScriptEngine {
public:
    /**
     * @brief Intializes Lua state and opens libs.
     */
    static void Init();

    /**
     * @brief Closes Lua state main thread.
     */
    static void CloseState();

    /**
     * @brief Loads Lua file script.
     *
     * @param filePath Path to file to be loaded.
     * @return true File loaded successfully.
     * @return false Failed to load file.
     */
    static bool LoadFile(const char* filePath);

    /**
     * @brief Runs Lua script snippet.
     *
     * @param code String to run.
     * @return true Success running script.
     * @return false Failed to run script.
     */
    static bool RunCode(const char* code);

    /**
     * @brief Get the Lua state object.
     *
     * @return const lua_State*
     */
    [[nodiscard]] static inline const lua_State* GetState() { return state; }

    /**
     * @brief Registers and exposes C++ class to Lua.
     *
     * @tparam T Class to be registered.
     * @param nameSpace Class namespace.
     */
    template <class T>
    static void RegisterClass(const char* nameSpace = nullptr) {
        LuaClass<T>::Register(state, nameSpace);
    }

    /**
     * @brief Registers and exposes C++ anonymous function to Lua.
     *
     * @param name Name of the function to be used in Lua.
     * @param fn Function pointer.
     */
    static void RegisterFunction(const char* name, lua_CFunction fn);

    /**
     * @brief Calls Lua funtion.
     *
     * @tparam Args Lua function argument types.
     * @param name Name of the Lua function.
     * @param args Lua function arguments.
     */
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
        lua_pop(state, 1);
    }

    /**
     * @brief Calls Lua funtion and saves return in lValue.
     *
     * @tparam T Return type for lValue.
     * @tparam Args Args Lua function argument types.
     * @param lValue Return from Lua function.
     * @param name Name of the Lua function.
     * @param args Lua function arguments.
     */
    template <typename T, typename... Args>
    static void CallFunction(T& lValue, const char* name, Args... args) {
        lua_getglobal(state, name);
        if (!lua_isfunction(state, -1)) {
            LOG_ENGINE_WARN("Tried to call an invalid Lua function: {}", name);
            lua_pop(state, 1);
            return;
        }
        PushValues(std::forward<Args>(args)...);
        lua_call(state, sizeof...(Args), 1);
        lValue = GetValue<T>(-1);
        lua_pop(state, 1);
    }

    /**
     * @brief Calls Lua function without arguments.
     *
     * @param name Name of the Lua function.
     */
    static void CallFunction(const char* name);

    /**
     * @brief Get the global variable from Lua.
     *
     * @tparam R Type of returned variable.
     * @param name Name of the variable.
     * @return R Lua global variable.
     */
    template <typename R>
    [[nodiscard]] static inline R GetGlobalVariable(const char* name) {
        lua_getglobal(state, name);
        const R r = GetValue<R>(-1);
        lua_pop(state, 1);
        return r;
    }

    /**
     * @brief Get value from Lua stack.
     *
     * @tparam R Value type.
     * @param index Stack index.
     * @return R
     */
    template <typename R>
    [[nodiscard]] static inline R GetValue(const int index = 1) {
        return MS::GetValue(lua::Type<R>{}, state, index);
    }

    /**
     * @brief Push value to Lua stack.
     *
     * @tparam T Value type.
     * @param value Value pushed to Lua stack.
     */
    template <typename T>
    static void PushValue(T value) {
        MS::PushValue(state, value);
    }

    /**
     * @brief Recursive helper method to push multiple values to Lua stack.
     *
     * @tparam T Value type.
     * @tparam Args Rest of the values types.
     * @param first Value pushed to Lua stack.
     * @param args Rest of the value to push to Lua stack.
     */
    template <typename T, typename... Args>
    static void PushValues(T first, Args... args) {
        PushValue(first);
        PushValues(std::forward<Args>(args)...);
    }

    /**
     * @brief Stop method to recursive PushValues overload.
     *
     * @tparam T Value type.
     * @param first Value pushed to Lua stack.
     */
    template <typename T>
    static void PushValues(T first) {
        PushValue(first);
    }

private:
    /**
     * @brief Lua state with static storage.
     */
    static lua_State* state;

    /**
     * @brief Checks for lua status and returns if ok or not.
     *
     * @param status Status code obtained from lua function
     * @param errMessage Default error message to print if none is obtained from lua stack
     * @return true Lua ok
     * @return false Something failed
     */
    static bool checkStatus(int status, const char* errMessage = "");
};
}  // namespace lua

/**
 * @brief Script engine abbreviation.
 */
using SE = lua::ScriptEngine;
