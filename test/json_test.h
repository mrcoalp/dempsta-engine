#pragma once

#include "JSON/json.h"

struct SimpleString : public de::JSON::Visitable {
    std::string string;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<SimpleString>& visitor) {
        visitor.Node(string, "string");
    }
};

bool test_json_parse_simple_string() {
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

bool test_json_parse_errors() {
    SimpleString other;
    std::ifstream stream{"assets/json/error.json"};
    return !de::JSON::ReadSilent(stream, other).IsEmpty();
}
