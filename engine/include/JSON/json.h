#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace de::JSON {
template <typename T>
struct Primitive : public std::false_type {};
template <>
struct Primitive<int> : public std::true_type {};
template <>
struct Primitive<unsigned> : public std::true_type {};
template <>
struct Primitive<float> : public std::true_type {};
template <>
struct Primitive<std::string> : public std::true_type {};
template <>
struct Primitive<bool> : public std::true_type {};

template <typename T>
using IsPrimitive = std::enable_if_t<Primitive<T>::value, int>;

template <typename T>
struct Array : public std::false_type {};
template <typename T>
struct Array<std::vector<T>> : public std::true_type {};

template <typename T>
using IsArray = std::enable_if_t<Array<T>::value, int>;

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

struct DummyVisitor {};

template <class T>
using IsVisitor = std::enable_if_t<std::is_base_of_v<DummyVisitor, T>, int>;

struct Vec2 : public DummyVisitor {
    float x;
    float y;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec2>& visitor) {
        visitor.Node(x, "x").Node(y, "y");
    }

    Vec2() = default;
    explicit Vec2(const glm::vec2& vec) : x(vec.x), y(vec.y) {}
};

struct Vec3 : public DummyVisitor {
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

struct Vec4 : public DummyVisitor {
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

template <typename>
class WriteVisitor {
public:
    explicit WriteVisitor(json& obj) : object(obj) {}

    template <typename Field>
    inline void Visit(Field& field) {
        visit(object, field);
    }

    template <typename Field>
    WriteVisitor& Node(Field& field, const std::string& name) {
        visit(object[name], field);
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
    json& object;

    template <typename Field>
    inline void visit(json& json, Field& field, IsVisitor<Field> = {}) {
        WriteVisitor<Field> visitor{json};
        field.Visit(visitor);
    }

    template <typename Field>
    inline void visit(json& json, Field& field, IsPrimitive<Field> = {}) {
        json = field;
    }

    template <typename Field>
    inline void visit(json& json, Field& field, IsArray<Field> = {}) {
        json = json::array();
        using VectorTpe = std::decay_t<decltype(*field.begin())>;
        for (auto& element : field) {
            json.emplace_back();
            WriteVisitor<VectorTpe> visitor{json.back()};
            visitor.Visit(element);
        }
    }
};

template <typename>
class ReadVisitor {
public:
    explicit ReadVisitor(json& obj) : object(obj) {}

    template <typename Field>
    inline void Visit(Field& field) {
        visit(object, field);
    }

    template <typename Field>
    ReadVisitor& Node(Field& field, const std::string& name) {
        auto it = object.find(name);
        if (it != object.end()) {
            if (!it->is_null()) {
                visit(*it, field);
            }
        }
        return *this;
    }

    template <typename Field>
    ReadVisitor& OptionalNode(Field& field, const std::string& name, const Field& defaultValue = {}) {
        auto it = object.find(name);
        field = defaultValue;
        if (it != object.end()) {
            if (!it->is_null()) {
                visit(*it, field);
            }
        }
        return *this;
    }

private:
    json& object;

    template <typename Field>
    inline void visit(json& json, Field& field, IsVisitor<Field> = {}) {
        ReadVisitor<Field> visitor{json};
        field.Visit(visitor);
    }

    template <typename Field>
    inline void visit(json& json, Field& field, IsPrimitive<Field> = {}) {
        try {
            field = json.get<Field>();
        } catch (const std::exception& e) {
        }
    }

    template <typename Field>
    inline void visit(json& json, Field& field, IsArray<Field> = {}) {
        using VectorTpe = std::decay_t<decltype(*field.begin())>;
        for (auto& element : json) {
            field.emplace_back();
            ReadVisitor<VectorTpe> visitor{element};
            visitor.Visit(field.back());
        }
    }
};

template <typename Source>
void WriteFile(Source& source, const std::string& filepath) {
    std::ofstream stream(filepath);
    json content;
    WriteVisitor<Source> visitor{content};
    visitor.Visit(source);
    stream << std::setw(4) << content << std::endl;
    if (!stream.good()) {
        // todo(mpinto): implement error handling
    }
}

template <class Destination>
void ReadFile(const std::string& filepath, Destination& dest) {
    std::ifstream stream(filepath);
    json content;
    stream >> content;
    ReadVisitor<Destination> visitor{content};
    visitor.Visit(dest);
}
}  // namespace de::JSON
