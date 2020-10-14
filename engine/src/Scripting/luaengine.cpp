#include "Scripting/luaengine.h"

#include <sstream>

#include "Core/log.h"

namespace lua {
lua_State* LuaEngine::state = nullptr;

void LuaEngine::Init() {
    state = luaL_newstate();
    luaL_openlibs(state);
    LuaFunction::Register(state);
}

void LuaEngine::CloseState() {
    lua_close(state);
    state = nullptr;
}

std::string LuaEngine::GetStackDump() {
    int top = lua_gettop(state);
    std::stringstream dump;
    dump << "***** LUA STACK *****" << std::endl;
    for (int i = 1; i <= top; ++i) {
        int t = lua_type(state, i);
        switch (t) {
            case LUA_TSTRING:
                dump << GetValue<const char*>(i) << std::endl;
                break;
            case LUA_TBOOLEAN:
                dump << std::boolalpha << GetValue<bool>(i) << std::endl;
                break;
            case LUA_TNUMBER:
                dump << GetValue<double>(i) << std::endl;
                break;
            default:  // NOTE(mpinto): Other values, print type
                dump << lua_typename(state, t) << std::endl;
                break;
        }
    }
    return dump.str();
}

bool LuaEngine::LoadFile(const char* filePath) {
    if (!checkStatus(luaL_loadfile(state, filePath), "Error loading file")) {
        return false;
    }
    return checkStatus(lua_pcall(state, 0, LUA_MULTRET, 0), "Loading file failed");
}

bool LuaEngine::RunCode(const char* code) {
    if (!checkStatus(luaL_loadstring(state, code), "Error running code")) {
        return false;
    }
    return checkStatus(lua_pcall(state, 0, LUA_MULTRET, 0), "Running code failed");
}

void LuaEngine::PushNull() { MS::PushNull(state); }

void LuaEngine::RegisterFunction(const char* name, LuaCFunction fn) { lua_register(state, name, fn); }

bool LuaEngine::CallFunction(const char* name) {
    lua_getglobal(state, name);
    if (!lua_isfunction(state, -1)) {
        lua_pop(state, 1);
        return false;
    }
    return checkStatus(lua_pcall(state, 0, 0, 0), "Failed to call LUA function");
}

bool LuaEngine::checkStatus(int status, const char* errMessage) {
    if (status != LUA_OK) {
        if (status == LUA_ERRSYNTAX) {
            const char* msg = lua_tostring(state, -1);
            LOG_ENGINE_ERROR(msg ? msg : errMessage);
        } else if (status == LUA_ERRFILE) {
            const char* msg = lua_tostring(state, -1);
            LOG_ENGINE_ERROR(msg ? msg : errMessage);
        }
        return false;
    }
    return true;
}
}  // namespace lua
