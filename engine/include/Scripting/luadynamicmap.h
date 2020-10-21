#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Scripting/types.h"

namespace lua {
struct LuaData {
    enum class Type { Double, Boolean, String, Map };
    std::string key;
    Type type;
};

class LuaDynamicMap {
public:
    LuaDynamicMap() = default;

    void Add(const std::string& key, double value, int depth) {
        bool found = false;
        auto type = LuaData::Type::Double;
        for (auto& data : m_tracker[depth]) {
            if (data.key == key) {
                data.type = type;
                m_doubles[depth][key] = value;
                found = true;
                break;
            }
        }
        if (!found) {
            m_tracker[depth].emplace_back(LuaData{key, type});
            m_doubles[depth].emplace(key, value);
        }
    }

    void Add(const std::string& key, bool value, int depth) {
        bool found = false;
        auto type = LuaData::Type::Boolean;
        for (auto& data : m_tracker[depth]) {
            if (data.key == key) {
                data.type = type;
                m_bools[depth][key] = value;
                found = true;
                break;
            }
        }
        if (!found) {
            m_tracker[depth].emplace_back(LuaData{key, type});
            m_bools[depth].emplace(key, value);
        }
    }

    void Add(const std::string& key, const std::string& value, int depth) {
        bool found = false;
        auto type = LuaData::Type::String;
        for (auto& data : m_tracker[depth]) {
            if (data.key == key) {
                data.type = type;
                m_strings[depth][key] = value;
                found = true;
                break;
            }
        }
        if (!found) {
            m_tracker[depth].emplace_back(LuaData{key, type});
            m_strings[depth].emplace(key, value);
        }
    }

    void Add(const std::string& key, int depth) {
        bool found = false;
        for (auto& data : m_tracker[depth]) {
            if (data.key == key) {
                data.type = LuaData::Type::Map;
                found = true;
                break;
            }
        }
        if (!found) {
            m_tracker[depth].emplace_back(LuaData{key, LuaData::Type::Map});
        }
    }

    [[nodiscard]] inline const auto& GetTracker() const { return m_tracker; }

    [[nodiscard]] inline double GetDouble(const std::string& key, int depth) const { return m_doubles.at(depth).at(key); }

    [[nodiscard]] inline bool GetBool(const std::string& key, int depth) const { return m_bools.at(depth).at(key); }

    [[nodiscard]] inline const std::string& GetString(const std::string& key, int depth) const { return m_strings.at(depth).at(key); }

private:
    template <typename T>
    using Buffer = std::unordered_map<int, std::unordered_map<std::string, T>>;

    std::map<int, std::vector<LuaData>> m_tracker;
    Buffer<double> m_doubles;
    Buffer<bool> m_bools;
    Buffer<std::string> m_strings;
};
}  // namespace lua
