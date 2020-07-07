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
    std::string rValue;
    SM::CallFunction(rValue, "OnUpdate", 0.016);
    SM::CloseState();
    return rValue == "FPS in Lua:62.5";
}

bool test_cpp_class_bind_lua() {
    SM::Init();
    SM::RegisterClass<Script>();
    SM::LoadFile("scripts/cppclass.lua");
    SM::CloseState();
    return testClass == 40;
}
