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

void LuaEngine::PrintStackDump() {
    int top = lua_gettop(state);
    for (int i = 1; i <= top; ++i) {
        int t = lua_type(state, i);
        LOG_ENGINE_TRACE("LUA STACK");
        switch (t) {
            case LUA_TSTRING:
                LOG_ENGINE_TRACE("{}", GetValue<const char*>(i));
                break;
            case LUA_TBOOLEAN:
                LOG_ENGINE_TRACE("{}", GetValue<bool>(i));
                break;
            case LUA_TNUMBER:
                LOG_ENGINE_TRACE("{}", GetValue<double>(i));
                break;
            default:  // NOTE(mpinto): Other values, print type
                LOG_ENGINE_TRACE("{}", lua_typename(state, t));
                break;
        }
    }
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
