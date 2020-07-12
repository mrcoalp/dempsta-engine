#include "Scripting/scriptengine.h"

#include "Core/log.h"

namespace lua {
lua_State* ScriptEngine::state = nullptr;

void ScriptEngine::Init() {
    state = luaL_newstate();
    luaL_openlibs(state);
}

void ScriptEngine::CloseState() {
    lua_close(state);
    state = nullptr;
}

bool ScriptEngine::LoadFile(const char* filePath) {
    int status = luaL_loadfile(state, filePath);
    if (!checkStatus(status, "Error loading file")) {
        return false;
    }

    status = lua_pcall(state, 0, LUA_MULTRET, 0);
    return checkStatus(status, "Loading file failed");
}

bool ScriptEngine::RunCode(const char* code) {
    int status = luaL_loadstring(state, code);
    if (!checkStatus(status, "Error running code")) {
        return false;
    }

    status = lua_pcall(state, 0, LUA_MULTRET, 0);
    return checkStatus(status, "Running code failed");
}

void ScriptEngine::RegisterFunction(const char* name, lua_CFunction fn) { lua_register(state, name, fn); }

void ScriptEngine::CallFunction(const char* name) {
    lua_getglobal(state, name);
    if (lua_isfunction(state, -1)) {
        lua_call(state, 0, 0);
    } else {
        LOG_ENGINE_WARN("Tried to call an invalid Lua function: {}", name);
    }
    lua_pop(state, 1);
}

bool ScriptEngine::checkStatus(int status, const char* errMessage) {
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
