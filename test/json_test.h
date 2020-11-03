#pragma once

#include "JSON/json.h"

struct SimpleString : public de::JSON::Visitable {
	std::string string;

	template<template<typename> class Visitor>
	inline void Visit(Visitor<SimpleString>& visitor) {
		visitor.Node(string, "string");
	}
};

bool test_json_parse_simple_string() {
	SimpleString one{{}, "simple string in json"};
	de::JSON::WriteFile(one, "json/string.json");
	SimpleString other;
	de::JSON::ReadFile("json/string.json", other);
	return other.string == "simple string in json";
}
