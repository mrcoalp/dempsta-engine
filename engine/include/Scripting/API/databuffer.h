#pragma once

#include <unordered_map>

#include "Scripting/binding.h"
#include "Scripting/scriptengine.h"

namespace lua {
/**
 * @brief Serves as buffer of data to kepp between updates in scripts.
 * A special Lua bind is created to get and set values as indexes.
 * The class will hold those values in data structures for later use inside scripts.
 *
 * @examples
 *
 * ```lua
 * function OnInit(data)
 *      data.name = "Marco"
 *      data.counter = 0
 * end
 *
 * function OnUpdate(data, delta)
 *      if (data.counter == 60) then
 *          print(data.name)
 *          data.counter = 0
 *      else
 *          data.counter = data.counter + 1
 *      end
 * end
 * ```
 */
class DataBuffer {
public:
    DataBuffer() = default;

    LUA_DECLARE_CLASS(DataBuffer)

    /**
     * @brief Registers class as metatable in Lua.
     *
     * @param L Lua state
     */
    static void Register(lua_State* L) {
        luaL_newmetatable(L, "DataBuffer");
        int metatable = lua_gettop(L);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, &LuaGetter);
        lua_settable(L, metatable);

        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, &LuaSetter);
        lua_settable(L, metatable);
    }

    /**
     * @brief Fetches index as key and calls internal getter.
     *
     * @param L Lua state.
     * @return int Number of values returned to Lua.
     */
    static int LuaGetter(lua_State* L) {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        auto* self = *SE::GetValue<DataBuffer**>();
        self->Get(SE::GetValue<std::string>(2));

        return 1;
    }

    /**
     * @brief Fetches index as key and value. Calls internal setter.
     *
     * @param L Lua state.
     * @return int Number of values returned to Lua.
     */
    static int LuaSetter(lua_State* L) {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        auto* self = *SE::GetValue<DataBuffer**>();

        switch (SE::GetValueType(3)) {  // Handle Lua type
            case LuaType::Number:
                self->Set(SE::GetValue<std::string>(2), SE::GetValue<double>(3));
                break;
            case LuaType::Boolean:
                self->Set(SE::GetValue<std::string>(2), SE::GetValue<bool>(3));
                break;
            case LuaType::String:
                self->Set(SE::GetValue<std::string>(2), SE::GetValue<std::string>(3));
                break;
            case LuaType::Null:
            case LuaType::LightUserData:
            case LuaType::Table:
            case LuaType::Function:
            case LuaType::UserData:
            case LuaType::Thread:
            default:
                break;
        }

        lua_pop(L, 2);
        lua_remove(L, 1);
        lua_remove(L, 1);

        return 0;
    }

    /**
     * @brief Sets new value in internal buffer.
     *
     * @param key Value name.
     * @param value Value to store.
     */
    void Set(const std::string& key, double value) {
        erase(key);
        m_doubles[key] = value;
    }

    /**
     * @brief Sets new value in internal buffer.
     *
     * @param key Value name.
     * @param value Value to store.
     */
    void Set(const std::string& key, bool value) {
        erase(key);
        m_bools[key] = value;
    }

    /**
     * @brief Sets new value in internal buffer.
     *
     * @param key Value name.
     * @param value Value to store.
     */
    void Set(const std::string& key, const std::string& value) {
        erase(key);
        m_strings[key] = value;
    }

    /**
     * @brief Pushes value with identifier key to Lua.
     *
     * @param key Value name to fetch.
     */
    void Get(const std::string& key) {
        if (m_doubles.find(key) != m_doubles.end()) {
            SE::PushValue(m_doubles.at(key));
        } else if (m_bools.find(key) != m_bools.end()) {
            SE::PushValue(m_bools.at(key));
        } else if (m_strings.find(key) != m_strings.end()) {
            SE::PushValue(m_strings.at(key));
        } else {
            SE::PushNull();
        }
    }

private:
    std::unordered_map<std::string, double> m_doubles;
    std::unordered_map<std::string, bool> m_bools;
    std::unordered_map<std::string, std::string> m_strings;

    /**
     * @brief Ensures the key is unique amongst all maps.
     *
     * @param key Key to delete from maps.
     */
    void erase(const std::string& key) {
        m_doubles.erase(key);
        m_bools.erase(key);
        m_strings.erase(key);
    }
};
}  // namespace lua
