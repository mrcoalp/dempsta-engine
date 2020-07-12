#pragma once

#include "dempsta.h"

int testClass = -1;

class Script {
public:
    explicit Script(lua_State*) : m_prop(SE::GetValue<int>()) {}

    LUA_DECLARE_CLASS(Script)

    LUA_PROPERTY(m_prop, int)

    LUA_PROXY_METHOD(Getter) {
        SE::PushValue(m_prop + SE::GetValue<int>());
        return 1;
    }

    LUA_PROXY_METHOD(Setter) {
        testClass = SE::GetValue<int>();
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
    testCPPFunction = SE::GetValue<std::string>();
    return 0;
}

bool test_call_cpp_function_from_lua() {
    SE::Init();
    SE::RegisterFunction("cppFunction", cppFunction);
    SE::LoadFile("scripts/cppfunctions.lua");
    SE::CloseState();
    return testCPPFunction == "passed";
}

bool test_call_lua_function_from_cpp() {
    SE::Init();
    SE::LoadFile("scripts/luafunctions.lua");
    std::string s;
    SE::CallFunction(s, "OnUpdate", "delta");
    int i;
    SE::CallFunction(i, "Maths", 2, 3, 4);
    SE::CloseState();
    return s == "FPS in Lua:delta" && i == 10;
}

bool test_cpp_class_bind_lua() {
    SE::Init();
    SE::RegisterClass<Script>();
    SE::LoadFile("scripts/cppclass.lua");
    SE::CloseState();
    return testClass == 40;
}

bool test_get_global_lua_var_from_cpp() {
    SE::Init();
    SE::LoadFile("scripts/luavariables.lua");
    const std::string s = SE::GetGlobalVariable<std::string>("string");
    const bool b = SE::GetGlobalVariable<bool>("bool");
    const int i = SE::GetGlobalVariable<int>("int");
    const float f = SE::GetGlobalVariable<float>("float");
    const double d = SE::GetGlobalVariable<double>("double");
    SE::CloseState();
    return s == "passed" && b && i == -1 && f == 12.6f && d == 3.14;
}

bool test_lua_run_code() {
    SE::Init();
    const bool status = SE::RunCode("a = 'passed';print('FROM LUA: run code ', a)");
    SE::CloseState();
    return status;
}
