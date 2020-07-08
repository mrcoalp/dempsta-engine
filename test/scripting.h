#pragma once

#include "dempsta.h"

int testClass = -1;

class Script {
public:
    explicit Script(lua_State*) : m_prop(SM::GetValue<int>()) {}

    LUA_DECLARE_CLASS(Script)

    LUA_PROPERTY(m_prop, int)

    LUA_PROXY_METHOD(Getter) {
        SM::PushValue(m_prop + SM::GetValue<int>());
        return 1;
    }

    LUA_PROXY_METHOD(Setter) {
        testClass = SM::GetValue<int>();
        return 0;
    }

private:
    int m_prop;
};

LUA_DEFINE_BINDING(Script)
LUA_ADD_PROPERTY(m_prop)
LUA_ADD_METHOD(Getter)
LUA_ADD_METHOD(Setter);

std::string testCPPFunction = "";
int cppFunction(lua_State* L) {
    testCPPFunction = SM::GetValue<std::string>();
    return 0;
}

bool test_call_cpp_function_from_lua() {
    SM::Init();
    SM::RegisterFunction("cppFunction", cppFunction);
    SM::LoadFile("scripts/cppfunctions.lua");
    SM::CloseState();
    return testCPPFunction == "passed";
}

bool test_call_lua_function_from_cpp() {
    SM::Init();
    SM::LoadFile("scripts/luafunctions.lua");
    std::string s;
    SM::CallFunction(s, "OnUpdate", "delta");
    int i;
    SM::CallFunction(i, "Maths", 2, 3, 4);
    SM::CloseState();
    return s == "FPS in Lua:delta" && i == 10;
}

bool test_cpp_class_bind_lua() {
    SM::Init();
    SM::RegisterClass<Script>();
    SM::LoadFile("scripts/cppclass.lua");
    SM::CloseState();
    return testClass == 40;
}

bool test_get_global_lua_var_from_cpp() {
    SM::Init();
    SM::LoadFile("scripts/luavariables.lua");
    const std::string s = SM::GetGlobalVariable<std::string>("string");
    const bool b = SM::GetGlobalVariable<bool>("bool");
    const int i = SM::GetGlobalVariable<int>("int");
    const float f = SM::GetGlobalVariable<float>("float");
    const double d = SM::GetGlobalVariable<double>("double");
    SM::CloseState();
    return s == "passed" && b && i == -1 && f == 12.6f && d == 3.14;
}
