#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

namespace de::JSON {
template <typename T>
struct Trivial : std::false_type {};
template <>
struct Trivial<int> : std::true_type {};
template <>
struct Trivial<unsigned> : std::true_type {};
template <>
struct Trivial<bool> : std::true_type {};
template <>
struct Trivial<float> : std::true_type {};
template <>
struct Trivial<double> : std::true_type {};
template <>
struct Trivial<const char*> : std::true_type {};
template <>
struct Trivial<std::string> : std::true_type {};
template <typename T>
using IsTrivial = std::enable_if_t<Trivial<T>::value, int>;

template <typename T>
struct Array : std::false_type {};
template <typename T>
struct Array<std::vector<T>> : std::true_type {};
template <typename T>
using IsArray = std::enable_if_t<Array<T>::value, int>;

template <typename T>
struct Map : std::false_type {};
template <typename T>
struct Map<std::unordered_map<std::string, T>> : std::true_type {};
template <typename T>
struct Map<std::map<std::string, T>> : std::true_type {};
template <typename T>
using IsMap = std::enable_if_t<Map<T>::value, int>;

/**
 * @brief Base struct to use custom visitable types.
 */
struct Visitable {
    /**
     * @brief Whether or not the visitable field is empty. Defaults to false, not empty. Useful to use with optional nodes.
     */
    bool is_empty{false};
};
template <class T>
using IsVisitable = std::enable_if_t<std::is_base_of_v<Visitable, T>, int>;

/**
 * @brief Check for emptiness of Visitable struct.
 *
 * @tparam Field Visitable to check.
 */
template <typename Field>
struct IsEmpty {
    inline static bool Value(Field& field) { return valueImpl(field, std::is_base_of<Visitable, Field>()); }

private:
    inline static bool valueImpl(Field& field, std::true_type) { return field.is_empty; }
    inline static bool valueImpl(Field& field, std::false_type) { return false; }
};

class ErrorStream {
public:
    explicit ErrorStream(std::vector<std::string>& destination) : m_destination(destination) {}

    ~ErrorStream() { m_destination.push_back(m_stream.str()); }

    template <typename Value>
    ErrorStream& operator<<(Value&& value) {
        m_stream << std::forward<Value>(value);
        return *this;
    }

private:
    std::stringstream m_stream;
    std::vector<std::string>& m_destination;
};

class ErrorTracking {
public:
    struct Node {
        std::string name;
        bool isArray;
    };

    struct Guard {
        ~Guard() { tracking.PopNode(); }
        ErrorTracking& tracking;
    };

    inline Guard PushNode(const std::string& name, bool isArray = false) {
        m_path.emplace_back(Node{name, isArray});
        return {*this};
    }

    inline void PopNode() { m_path.pop_back(); }

    inline ErrorStream Error() { return ErrorStream(m_errors); }

    [[nodiscard]] std::string GetPath() const {
        std::stringstream path;
        path << "ROOT";
        for (const auto& node : m_path) {
            if (node.isArray) {
                path << "[" << node.name << "]";
                continue;
            }
            path << "." << node.name;
        }
        return path.str();
    }

    [[nodiscard]] inline bool IsEmpty() const { return m_errors.empty(); }

    [[nodiscard]] std::vector<std::string>::iterator begin() { return m_errors.begin(); }
    [[nodiscard]] std::vector<std::string>::iterator end() { return m_errors.end(); }
    [[nodiscard]] std::vector<std::string>::const_iterator begin() const { return m_errors.cbegin(); }
    [[nodiscard]] std::vector<std::string>::const_iterator end() const { return m_errors.cend(); }

private:
    std::vector<Node> m_path;
    std::vector<std::string> m_errors;
};

template <typename>
class WriteVisitor {
public:
    explicit WriteVisitor(json& object) : m_object(object) {}

    template <typename Field>
    inline void Visit(Field& field) {
        visit(m_object, field);
    }

    template <typename Field>
    WriteVisitor& Node(Field& field, const std::string& name) {
        visit(m_object[name], field);
        return *this;
    }

    template <typename Field>
    WriteVisitor& OptionalNode(Field& field, const std::string& name, Field = {}) {
        if (!IsEmpty<Field>::Value(field)) {
            return Node(field, name);
        }
        return *this;
    }

private:
    json& m_object;

    template <typename Field, IsVisitable<Field> = 0>
    inline void visit(json& json, Field& field) {
        WriteVisitor<Field> visitor{json};
        field.Visit(visitor);
    }

    template <typename Field, IsTrivial<Field> = 0>
    inline void visit(json& json, Field& field) {
        json = field;
    }

    template <typename Field, IsArray<Field> = 0>
    inline void visit(json& json, Field& field) {
        json = json::array();
        using VectorType = std::decay_t<decltype(*field.begin())>;
        for (auto& element : field) {
            json.emplace_back();
            WriteVisitor<VectorType> visitor{json.back()};
            visitor.Visit(element);
        }
    }

    template <typename Field, IsMap<Field> = 0>
    inline void visit(json& json, Field& field) {
        using MapType = std::decay_t<decltype(*field.begin().second)>;
        for (auto& pair : field) {
            auto& member = json[pair.first];
            WriteVisitor<MapType> visitor{member};
            visitor.Visit(pair.second);
        }
    }
};

template <typename>
class ReadVisitor {
public:
    explicit ReadVisitor(json& object, ErrorTracking& errors) : m_object(object), m_errors(errors) {}

    template <typename Field>
    inline void Visit(Field& field) {
        visit(m_object, field);
    }

    template <typename Field>
    ReadVisitor& Node(Field& field, const std::string& name) {
        auto it = m_object.find(name);
        if (it != m_object.end()) {
            if (!it->is_null()) {
                auto guard = m_errors.PushNode(name);
                visit(*it, field);
            }
        } else {
            m_errors.Error() << "Missing mandatory field '" << name << "' from '" << m_errors.GetPath() << "'";
        }
        return *this;
    }

    template <typename Field>
    ReadVisitor& OptionalNode(Field& field, const std::string& name, const Field& defaultValue = {}) {
        auto it = m_object.find(name);
        if (it != m_object.end()) {
            if (!it->is_null()) {
                if (IsEmpty<Field>::Value(field)) {
                    field = {false};
                }
                auto guard = m_errors.PushNode(name);
                visit(*it, field);
            }
        } else {
            field = defaultValue;
        }
        return *this;
    }

private:
    json& m_object;
    ErrorTracking& m_errors;

    template <typename Field, IsVisitable<Field> = 0>
    inline void visit(json& json, Field& field) {
        ReadVisitor<Field> visitor{json, m_errors};
        field.Visit(visitor);
    }

    template <typename Field, IsTrivial<Field> = 0>
    inline void visit(json& json, Field& field) {
        try {
            field = json.get<Field>();
        } catch (const std::exception& e) {
            m_errors.Error() << "Failed parsing json field '" << m_errors.GetPath() << "' with exception: " << e.what();
        }
    }

    template <typename Field, IsArray<Field> = 0>
    inline void visit(json& json, Field& field) {
        using VectorType = std::decay_t<decltype(*field.begin())>;
        for (size_t i = 0; i < json.size(); ++i) {
            auto& element = json.at(i);
            auto guard = m_errors.PushNode(std::to_string(i), true);
            field.emplace_back();
            ReadVisitor<VectorType> visitor{element, m_errors};
            visitor.Visit(field.back());
        }
    }

    template <typename Field, IsMap<Field> = 0>
    inline void visit(json& json, Field& field) {
        using MapType = std::decay_t<decltype(*field.begin().second)>;
        for (auto it = json.begin(); it != json.end(); ++it) {
            auto guard = m_errors.PushNode(it.key());
            ReadVisitor<MapType> visitor{it.value(), m_errors};
            visitor.Visit(field.at(it.key()));
        }
    }
};

template <typename Source>
bool WriteFile(Source& source, const std::string& filepath) {
    std::ofstream stream(filepath);
    json content;
    WriteVisitor<Source> visitor{content};
    visitor.Visit(source);
    stream << std::setw(4) << content << std::endl;
    return stream.good();
}

template <class Destination>
ErrorTracking ReadSilent(std::ifstream& stream, Destination& dest) {
    json content;
    ErrorTracking errors;
    try {
        stream >> content;
    } catch (const std::exception& e) {
        errors.Error() << e.what();
        return errors;
    }
    ReadVisitor<Destination> visitor{content, errors};
    visitor.Visit(dest);
    return errors;
}

template <class Destination>
bool ReadFile(const std::string& filepath, Destination& dest) {
    std::ifstream stream(filepath);
    auto errors = ReadSilent(stream, dest);
    for (const auto& error : errors) {
        LOG_ENGINE_ERROR("Error while parsing '{}': {}", filepath, error);
    }
    return errors.IsEmpty();
}
}  // namespace de::JSON
