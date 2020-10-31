#pragma once

#include "Scripting/API/databuffer.h"
#include "dempsta.h"

int testClass = -1;
#ifdef RegisterClass
#undef RegisterClass
#endif

class Script {
public:
    explicit Script(int prop) : m_prop(prop) {}

    explicit Script(lua_State*) : m_prop(LE::GetValue<int>()) {}

    LUA_DECLARE_CLASS(Script)

    LUA_PROPERTY(m_prop, int)

    LUA_METHOD(Getter) {
        LE::PushValue(m_prop + LE::GetValue<int>());
        return 1;
    }

    LUA_METHOD(Setter) {
        testClass = m_prop = LE::GetValue<int>();
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

std::string testCPPFunction;
LUA_METHOD(cppFunction) {
    testCPPFunction = LE::GetValue<std::string>();
    return 0;
}

bool test_call_cpp_function_from_lua() {
    LE::Init();
    LE::RegisterFunction("cppFunction", cppFunction);
    LE::LoadFile("scripts/cppfunctions.lua");
    LE::CloseState();
    return testCPPFunction == "passed";
}

bool test_call_lua_function_pass_binding_object_modify_inside_lua() {
    Script o(20);
    LE::Init();
    LE::RegisterClass<Script>();
    LE::LoadFile("scripts/luafunctions.lua");
    if (!LE::CallFunction("Object", &o)) {
        return false;
    }
    LE::CloseState();
    return o.GetProp() == 1;
}

bool test_call_lua_function_pass_vector_get_string() {
    std::vector<std::string> vec;
    std::string s;
    LE::Init();
    LE::LoadFile("scripts/luafunctions.lua");
    for (size_t i = 0; i < 100; ++i) {
        vec.push_back(std::to_string(i));
    }
    if (!LE::CallFunction(s, "OnUpdate", vec, 100)) {
        return false;
    }
    LE::CloseState();
    return s == "99";
}

bool test_call_lua_function_pass_multiple_params_get_int() {
    int i;
    LE::Init();
    LE::LoadFile("scripts/luafunctions.lua");
    if (!LE::CallFunction(i, "Maths", 2, 3, 4)) {
        return false;
    }
    LE::CloseState();
    return i == 10;
}

bool test_call_lua_function_pass_multiple_params_get_vector() {
    std::vector<double> vecRet;
    LE::Init();
    LE::LoadFile("scripts/luafunctions.lua");
    if (!LE::CallFunction(vecRet, "VecTest", 2.2, 3.14, 4.0)) {
        return false;
    }
    LE::CloseState();
    return vecRet.size() == 3 && vecRet[1] == 3.14;
}

bool test_call_lua_function_get_anonymous_function_and_call_it() {
    lua::LuaFunction fun;
    LE::Init();
    LE::RegisterFunction("cppFunction", cppFunction);
    LE::LoadFile("scripts/luafunctions.lua");
    if (!LE::CallFunction(fun, "TestCallback")) {
        return false;
    }
    if (!fun()) {
        return false;
    }
    fun.Unload();
    LE::CloseState();
    return testCPPFunction == "passed_again";
}

bool test_cpp_class_bind_lua() {
    LE::Init();
    LE::RegisterClass<Script>();
    LE::LoadFile("scripts/cppclass.lua");
    LE::CloseState();
    return testClass == 40;
}

bool test_get_global_lua_var_from_cpp() {
    LE::Init();
    LE::LoadFile("scripts/luavariables.lua");
    const std::string s = LE::GetGlobalVariable<std::string>("string");
    const bool b = LE::GetGlobalVariable<bool>("bool");
    const int i = LE::GetGlobalVariable<int>("int");
    const float f = LE::GetGlobalVariable<float>("float");
    const double d = LE::GetGlobalVariable<double>("double");
    LE::CloseState();
    return s == "passed" && b && i == -1 && f == 12.6f && d == 3.14;
}

bool test_lua_run_code() {
    LE::Init();
    const bool status = LE::RunCode("a = 'passed'");
    const std::string s = LE::GetGlobalVariable<std::string>("a");
    LE::CloseState();
    return status && s == "passed";
}

std::string dataStored;

LUA_METHOD(NewMessage) {
    auto id = LE::GetValue<std::string>();
    auto* data = *LE::GetValue<lua::DataBuffer**>(2);
    LE::CallFunction(dataStored, "Listen", id, data);
    dataStored = id + "_" + dataStored;
    delete data;
    return 0;
}

bool test_lua_data_buffer() {
    LE::Init();
    lua::DataBuffer::Register();
    LE::RegisterFunction("NewMessage", NewMessage);
    LE::LoadFile("scripts/databuffer.lua");
    LE::CallFunction("Broadcast");
    LE::CloseState();
    return dataStored == "msg_sender";
}

bool test_get_dynamic_map_from_lua() {
    LE::Init();
    LE::LoadFile("scripts/luafunctions.lua");
    lua::LuaDynamicMap map;
    if (!LE::CallFunction(map, "GetMap")) {
        return false;
    }
    bool value = false;
    if (!LE::CallFunction(value, "GetMapValue", map)) {
        return false;
    }
    map.Unload();
    LE::CloseState();
    // Depth must be 2
    return value;
}
