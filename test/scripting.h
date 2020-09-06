#pragma once

#include "dempsta.h"
#include "Scripting/API/databuffer.h"

int testClass = -1;

class Script {
public:
    explicit Script(int prop) : m_prop(prop) {}

    explicit Script(lua_State*) : m_prop(SE::GetValue<int>()) {}

    LUA_DECLARE_CLASS(Script)

    LUA_PROPERTY(m_prop, int)

    LUA_METHOD(Getter) {
        SE::PushValue(m_prop + SE::GetValue<int>());
        return 1;
    }

    LUA_METHOD(Setter) {
        testClass = m_prop = SE::GetValue<int>();
        return 0;
    }

    [[nodiscard]] inline int GetProp() const { return m_prop; }

private:
    int m_prop;
};

LUA_DEFINE_BINDING(Script, false)
LUA_ADD_PROPERTY(m_prop)
LUA_ADD_METHOD(Getter)
LUA_ADD_METHOD(Setter);

std::string testCPPFunction = "";
LUA_METHOD(cppFunction) {
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
    Script o(20);
    std::vector<std::string> vec;
    std::string s;
    int i;
    std::vector<double> vecRet;
    SE::Init();
    SE::RegisterClass<Script>();
    SE::LoadFile("scripts/luafunctions.lua");
    if (!SE::CallFunction("Object", &o)) {
        return false;
    }
    for (size_t i = 0; i < 100; ++i) {
        vec.push_back(std::to_string(i));
    }
    if (!SE::CallFunction(s, "OnUpdate", vec, 100)) {
        return false;
    }
    if (!SE::CallFunction(i, "Maths", 2, 3, 4)) {
        return false;
    }
    if (!SE::CallFunction(vecRet, 3, "VecTest", 2.2, 3.14, 4.0)) {
        return false;
    }
    SE::CloseState();
    return s == "99" && i == 10 && o.GetProp() == 1 && vecRet.size() == 3 && vecRet[1] == 3.14;
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
    const bool status = SE::RunCode("a = 'passed'");
    const std::string s = SE::GetGlobalVariable<std::string>("a");
    SE::CloseState();
    return status && s == "passed";
}

std::string dataStored;

LUA_METHOD(NewMessage) {
    auto id = SE::GetValue<std::string>();
    auto* data = *SE::GetValue<lua::DataBuffer**>(2);
    SE::CallFunction(dataStored, "Listen", id, data);
    dataStored = id + "_" + dataStored;
    delete data;
    return 0;
}

bool test_lua_data_buffer() {
    SE::Init();
    lua::DataBuffer::Register();
    SE::RegisterFunction("NewMessage", NewMessage);
    SE::LoadFile("scripts/databuffer.lua");
    SE::CallFunction("Broadcast");
    SE::CloseState();
    return dataStored == "msg_sender";
}
