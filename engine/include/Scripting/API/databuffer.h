#pragma once

#include <map>

#include "Scripting/binding.h"
#include "Scripting/scriptengine.h"

namespace lua {
class DataBuffer {
public:
    DataBuffer() = default;

    explicit DataBuffer(lua_State*) {}

    LUA_DECLARE_CLASS(DataBuffer)

    LUA_PROXY_METHOD(Set) {
        auto key = SE::GetValue<std::string>();
        erase(key);
        switch (SE::GetValueType(2)) {
            case LuaType::Number:
                m_doubles[key] = SE::GetValue<double>(2);
                break;
            case LuaType::Boolean:
                m_bools[key] = SE::GetValue<bool>(2);
                break;
            case LuaType::String:
                m_strings[key] = SE::GetValue<std::string>(2);
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
        return 0;
    }

    LUA_PROXY_METHOD(Get) {
        auto key = SE::GetValue<std::string>();
        if (m_doubles.find(key) != m_doubles.end()) {
            SE::PushValue(m_doubles.at(key));
        } else if (m_bools.find(key) != m_bools.end()) {
            SE::PushValue(m_bools.at(key));
        } else if (m_strings.find(key) != m_strings.end()) {
            SE::PushValue(m_strings.at(key));
        } else {
            SE::PushNull();
        }
        return 1;
    }

private:
    std::unordered_map<std::string, double> m_doubles;
    std::unordered_map<std::string, bool> m_bools;
    std::unordered_map<std::string, std::string> m_strings;

    void erase(const std::string& key) {
        m_doubles.erase(key);
        m_bools.erase(key);
        m_strings.erase(key);
    }
};
}  // namespace lua
