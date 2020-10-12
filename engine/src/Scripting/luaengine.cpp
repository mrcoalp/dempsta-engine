#include "Scripting/luaengine.h"

#include "Core/log.h"

namespace lua {
lua_State* LuaEngine::state = nullptr;

void LuaEngine::Init() {
    state = luaL_newstate();
    luaL_openlibs(state);
}

void LuaEngine::CloseState() {
    lua_close(state);
    state = nullptr;
}

bool LuaEngine::LoadFile(const char* filePath) {
    int status = luaL_loadfile(state, filePath);
    if (!checkStatus(status, "Error loading file")) {
        return false;
    }
    status = lua_pcall(state, 0, LUA_MULTRET, 0);
    return checkStatus(status, "Loading file failed");
}

bool LuaEngine::RunCode(const char* code) {
    int status = luaL_loadstring(state, code);
    if (!checkStatus(status, "Error running code")) {
        return false;
    }
    status = lua_pcall(state, 0, LUA_MULTRET, 0);
    return checkStatus(status, "Running code failed");
}

void LuaEngine::PushNull() { MS::PushNull(state); }

void LuaEngine::RegisterFunction(const char* name, LuaCFunction fn) { lua_register(state, name, fn); }

bool LuaEngine::CallFunction(const char* name) {
    lua_getglobal(state, name);
    if (lua_isfunction(state, -1)) {
        lua_call(state, 0, 0);
        return true;
    }
    lua_pop(state, 1);
    return false;
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
