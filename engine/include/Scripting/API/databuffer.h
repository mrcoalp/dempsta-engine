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

    LUA_PROXY_METHOD(SetInt) {
        m_ints[SE::GetValue<std::string>()] = SE::GetValue<int>(2);
        return 0;
    }

    LUA_PROXY_METHOD(GetInt) {
        auto key = SE::GetValue<std::string>();
        if (m_ints.find(key) == m_ints.end()) {
            SE::PushNull();
        } else {
            SE::PushValue(m_ints.at(key));
        }
        return 1;
    }

    LUA_PROXY_METHOD(SetFloat) {
        m_floats[SE::GetValue<std::string>()] = SE::GetValue<float>(2);
        return 0;
    }

    LUA_PROXY_METHOD(GetFloat) {
        auto key = SE::GetValue<std::string>();
        if (m_floats.find(key) == m_floats.end()) {
            SE::PushNull();
        } else {
            SE::PushValue(m_floats.at(key));
        }
        return 1;
    }

    LUA_PROXY_METHOD(SetDouble) {
        m_doubles[SE::GetValue<std::string>()] = SE::GetValue<double>(2);
        return 0;
    }

    LUA_PROXY_METHOD(GetDouble) {
        auto key = SE::GetValue<std::string>();
        if (m_doubles.find(key) == m_doubles.end()) {
            SE::PushNull();
        } else {
            SE::PushValue(m_doubles.at(key));
        }
        return 1;
    }

    LUA_PROXY_METHOD(SetBool) {
        m_bools[SE::GetValue<std::string>()] = SE::GetValue<bool>(2);
        return 0;
    }

    LUA_PROXY_METHOD(GetBool) {
        auto key = SE::GetValue<std::string>();
        if (m_bools.find(key) == m_bools.end()) {
            SE::PushNull();
        } else {
            SE::PushValue(m_bools.at(key));
        }
        return 1;
    }

    LUA_PROXY_METHOD(SetString) {
        m_strings[SE::GetValue<std::string>()] = SE::GetValue<std::string>(2);
        return 0;
    }

    LUA_PROXY_METHOD(GetString) {
        auto key = SE::GetValue<std::string>();
        if (m_strings.find(key) == m_strings.end()) {
            SE::PushNull();
        } else {
            SE::PushValue(m_strings.at(key));
        }
        return 1;
    }

private:
    std::unordered_map<std::string, int> m_ints;
    std::unordered_map<std::string, float> m_floats;
    std::unordered_map<std::string, double> m_doubles;
    std::unordered_map<std::string, bool> m_bools;
    std::unordered_map<std::string, std::string> m_strings;
};
}  // namespace lua
