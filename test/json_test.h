#pragma once

#include "JSON/json.h"

struct SimpleString : public de::JSON::Visitable {
    std::string string;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<SimpleString>& visitor) {
        visitor.Node(string, "string");
    }
};

struct Array : public de::JSON::Visitable {
    std::vector<SimpleString> strings;
    std::vector<bool> bits;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Array>& visitor) {
        visitor.Node(strings, "strings").Node(bits, "bits");
    }
};

struct Map : public de::JSON::Visitable {
    std::unordered_map<std::string, SimpleString> strings;
    std::map<std::string, float> floats;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Map>& visitor) {
        visitor.Node(strings, "strings").Node(floats, "floats");
    }
};

struct Complex : public de::JSON::Visitable {
    std::map<std::string, bool> stdMap;
    Array array;
    float f;
    Map map{{true}};

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Complex>& visitor) {
        visitor.Node(stdMap, "std_map").Node(array, "array").Node(f, "float").OptionalNode(map, "map", {{true}});
    }
};

bool test_json_parse_simple_string_struct() {
    SimpleString one{{}, "simple string in json"};
    if (!de::JSON::WriteFile(one, "assets/json/string.json")) {
        return false;
    }
    SimpleString other;
    if (!de::JSON::ReadFile("assets/json/string.json", other)) {
        return false;
    }
    return other.string == "simple string in json";
}

bool test_json_parse_array() {
    Array array;
    for (size_t i = 0; i < 10; ++i) {
        array.strings.emplace_back(SimpleString{{false}, std::string("simple string in json ").append(std::to_string(i))});
        array.bits.emplace_back(i % 2 == 0);
    }
    if (!de::JSON::WriteFile(array, "assets/json/array.json")) {
        return false;
    }
    Array other;
    if (!de::JSON::ReadFile("assets/json/array.json", other)) {
        return false;
    }
    return other.strings[2].string == "simple string in json 2" && !other.bits[1];
}

bool test_json_parse_map() {
    Map map;
    for (size_t i = 0; i < 10; ++i) {
        map.strings.emplace(std::to_string(i), SimpleString{{false}, std::string("simple string in json ").append(std::to_string(i))});
        map.floats.emplace(std::to_string(i), i * 2.f);
    }
    if (!de::JSON::WriteFile(map, "assets/json/map.json")) {
        return false;
    }
    Map other;
    if (!de::JSON::ReadFile("assets/json/map.json", other)) {
        return false;
    }
    return other.strings.at("2").string == "simple string in json 2" && other.floats.at("4") == 8.f;
}

bool test_json_parse_complex_struct() {
    Array array;
    std::map<std::string, bool> stdMap;
    for (size_t i = 0; i < 10; ++i) {
        array.strings.emplace_back(SimpleString{{false}, std::string("simple string in json ").append(std::to_string(i))});
        array.bits.emplace_back(i % 2 == 0);
        stdMap.emplace(std::to_string(i), i % 2 != 0);
    }
    Complex complex{{false}, stdMap, array, 0.6f};
    if (!de::JSON::WriteFile(complex, "assets/json/complex.json")) {
        return false;
    }
    Complex other;
    if (!de::JSON::ReadFile("assets/json/complex.json", other)) {
        return false;
    }
    return other.array.strings[2].string == "simple string in json 2" && other.stdMap.at("5") && other.f == 0.6f && other.map.is_empty;
}

bool test_json_parse_errors() {
    SimpleString other;
    std::ifstream stream{"assets/json/error.json"};
    return !de::JSON::ReadSilent(stream, other).IsEmpty();
}
