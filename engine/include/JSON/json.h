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
struct Trivial<float> : std::true_type {};
template <>
struct Trivial<std::string> : std::true_type {};
template <>
struct Trivial<bool> : std::true_type {};
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

template <typename T>
using IsEmptyCall = decltype(std::declval<T>().is_empty());
template <typename T, typename = void>
struct IsEmpty {
    inline static bool Value(T& field) { return false; }
};
template <typename T>
struct IsEmpty<T, IsEmptyCall<T>> {
    inline static bool Value(T& field) { return field.is_empty(); }
};

struct Visitable {};
template <class T>
using IsVisitable = std::enable_if_t<std::is_base_of_v<Visitable, T>, int>;

struct Vec2 : public Visitable {
    float x;
    float y;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec2>& visitor) {
        visitor.Node(x, "x").Node(y, "y");
    }

    Vec2() = default;
    explicit Vec2(const glm::vec2& vec) : x(vec.x), y(vec.y) {}
};

struct Vec3 : public Visitable {
    float x;
    float y;
    float z;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec3>& visitor) {
        visitor.Node(x, "x").Node(y, "y").Node(z, "z");
    }

    Vec3() = default;
    explicit Vec3(const glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}
};

struct Vec4 : public Visitable {
    float x;
    float y;
    float z;
    float w;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec4>& visitor) {
        visitor.Node(x, "x").Node(y, "y").Node(z, "z").Node(w, "w");
    }

    Vec4() = default;
    explicit Vec4(const glm::vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
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

    inline void AddError(std::string&& error) { m_errors.emplace_back(std::forward<std::string>(error)); }

    [[nodiscard]] std::string GetPath() const {
        std::stringstream path;
        for (size_t i = 0; i < m_path.size(); ++i) {
            const auto& node = m_path[i];
            if (node.isArray) {
                path << "[" << node.name << "]";
                continue;
            }
            if (i > 0) {
                path << ".";
            }
            path << node.name;
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
        if (!IsEmpty<Field>::Value(field)) {  // todo(mpinto): fix this
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
            std::stringstream error;
            error << "Missing mandatory field '" << name << "' from '" << m_errors.GetPath() << "'";
            m_errors.AddError(error.str());
        }
        return *this;
    }

    template <typename Field>
    ReadVisitor& OptionalNode(Field& field, const std::string& name, const Field& defaultValue = {}) {
        auto it = m_object.find(name);
        field = defaultValue;
        if (it != m_object.end()) {
            if (!it->is_null()) {
                auto guard = m_errors.PushNode(name);
                visit(*it, field);
            }
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
            std::stringstream error;
            error << "Failed parsing json field '" << m_errors.GetPath() << "' with exception: " << e.what();
            m_errors.AddError(error.str());
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
        errors.AddError(e.what());
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
