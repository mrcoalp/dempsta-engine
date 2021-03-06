#pragma once

#include <moon/moon.h>

#include <unordered_map>

namespace lua {
/**
 * @brief Serves as buffer of data to keep between updates in scripts.
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
    template <typename T>
    using Data = std::map<std::string, T>;

    DataBuffer() = default;

    explicit DataBuffer(lua_State*) {}

    MOON_DECLARE_CLASS(DataBuffer)

    /**
     * @brief Registers class as metatable in Lua.
     */
    static void Register() {
        auto* L = Moon::GetState();
        lua_pushcfunction(L, &LuaCtor);
        lua_setglobal(L, "DataBuffer");

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
     * @brief Construct a new lua data buffer object inside Lua
     *
     * @warning Use carefully, since lua does not handle garbage collection for this class.
     * Ensure a proper object deletion in C++ land when constructing inside lua.
     *
     * @param L Lua state.
     * @return int Number of values returned to Lua.
     */
    static MOON_METHOD(LuaCtor) {
        auto* buffer = new DataBuffer(L);
        Moon::PushValue(buffer);
        return 1;
    }

    /**
     * @brief Fetches index as key and calls internal getter.
     *
     * @param L Lua state.
     * @return int Number of values returned to Lua.
     */
    static MOON_METHOD(LuaGetter) {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        auto* self = *Moon::GetValue<DataBuffer**>();
        self->Get(Moon::GetValue<std::string>(2));

        return 1;
    }

    /**
     * @brief Fetches index as key and value. Calls internal setter.
     *
     * @param L Lua state.
     * @return int Number of values returned to Lua.
     */
    static MOON_METHOD(LuaSetter) {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        auto* self = *Moon::GetValue<DataBuffer**>();

        switch (Moon::GetValueType(3)) {  // Handle Lua type
            case moon::LuaType::Number:
                self->Set(Moon::GetValue<std::string>(2), Moon::GetValue<double>(3));
                break;
            case moon::LuaType::Boolean:
                self->Set(Moon::GetValue<std::string>(2), Moon::GetValue<bool>(3));
                break;
            case moon::LuaType::String:
                self->Set(Moon::GetValue<std::string>(2), Moon::GetValue<std::string>(3));
                break;
            case moon::LuaType::Null:
            case moon::LuaType::LightUserData:
            case moon::LuaType::Table:
            case moon::LuaType::Function:
            case moon::LuaType::UserData:
            case moon::LuaType::Thread:
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
            Moon::PushValue(m_doubles.at(key));
        } else if (m_bools.find(key) != m_bools.end()) {
            Moon::PushValue(m_bools.at(key));
        } else if (m_strings.find(key) != m_strings.end()) {
            Moon::PushValue(m_strings.at(key));
        } else {
            Moon::PushNull();
        }
    }

    std::tuple<Data<double>&, Data<bool>&, Data<std::string>&> GetData() { return std::tie(m_doubles, m_bools, m_strings); }

private:
    Data<double> m_doubles;
    Data<bool> m_bools;
    Data<std::string> m_strings;

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
