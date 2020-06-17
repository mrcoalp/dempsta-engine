#pragma once

#include <cstring>
#include <lua.hpp>

namespace lua {
//==== LunaLuaFive modded ====//
template <class T>
class LuaClass {
public:
    struct PropertyType {
        const char* name;

        int (T::*getter)(lua_State*);

        int (T::*setter)(lua_State*);
    };

    struct FunctionType {
        const char* name;

        int (T::*func)(lua_State*);
    };

    /*
      @ Check
      Arguments:
        * L - Lua State
        * narg - Position to check

      Description:
        Retrieves a wrapped class from the arguments passed to the func, specified by narg (position).
        This func will raise an exception if the argument is not of the correct type.
    */
    static T* Check(lua_State* L, int narg) {
        T** obj = static_cast<T**>(luaL_checkudata(L, narg, T::Binding.GetName()));
        if (!obj) return nullptr;  // LightCheck returns nullptr if not found.
        return *obj;               // pointer to T object
    }

    /*
      @ LightCheck
      Arguments:
        * L - Lua State
        * narg - Position to check

      Description:
        Retrieves a wrapped class from the arguments passed to the func, specified by narg (position).
        This func will return nullptr if the argument is not of the correct type.  Useful for supporting
        multiple types of arguments passed to the func
    */
    static T* LightCheck(lua_State* L, int narg) {
        T** obj = static_cast<T**>(luaL_testudata(L, narg, T::Binding.GetName()));
        if (!obj) return nullptr;  // LightCheck returns nullptr if not found.
        return *obj;               // pointer to T object
    }

    /*
      @ Register
      Arguments:
        * L - Lua State
        * nameSpace - Namespace to load into

      Description:
        Registers your class with Lua.  Leave nameSpace "" if you want to load it into the global space.
    */
    static void Register(lua_State* L, const char* nameSpace = nullptr) {
        if (nameSpace && strlen(nameSpace)) {
            lua_getglobal(L, nameSpace);
            if (lua_isnil(L, -1))  // Create namespace if not present
            {
                lua_newtable(L);
                lua_pushvalue(L, -1);  // Duplicate table pointer since setglobal pops the value
                lua_setglobal(L, nameSpace);
            }
            lua_pushcfunction(L, &LuaClass<T>::constructor);
            lua_setfield(L, -2, T::Binding.GetName());
            lua_pop(L, 1);
        } else {
            lua_pushcfunction(L, &LuaClass<T>::constructor);
            lua_setglobal(L, T::Binding.GetName());
        }

        luaL_newmetatable(L, T::Binding.GetName());
        int metatable = lua_gettop(L);

        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, &LuaClass<T>::gc_obj);
        lua_settable(L, metatable);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, &LuaClass<T>::to_string);
        lua_settable(L, metatable);
        // To be able to compare two Luna objects (not natively possible with full userdata)
        lua_pushstring(L, "__eq");
        lua_pushcfunction(L, &LuaClass<T>::equals);
        lua_settable(L, metatable);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, &LuaClass<T>::property_getter);
        lua_settable(L, metatable);

        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, &LuaClass<T>::property_setter);
        lua_settable(L, metatable);

        unsigned i = 0;
        for (const auto& property : T::Binding.GetProperties()) {  // Register some properties in it
            lua_pushstring(L, property.name);                      // Having some string associated with them
            lua_pushnumber(L, i);                                  // And an index of which property it is
            lua_settable(L, metatable);
            ++i;
        }

        i = 0;
        for (const auto& method : T::Binding.GetMethods()) {
            lua_pushstring(L, method.name);     // Register some functions in it
            lua_pushnumber(L, i | (1u << 8u));  // Add a number indexing which func it is
            lua_settable(L, metatable);
            ++i;
        }
    }

private:
    /*
      @ constructor (internal)
      Arguments:
        * L - Lua State
    */
    static int constructor(lua_State* L) {
        T* ap = new T(L);
        T** a = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));  // Push value = userdata
        *a = ap;

        luaL_getmetatable(L, T::Binding.GetName());  // Fetch global metatable T::classname
        lua_setmetatable(L, -2);
        return 1;
    }

    /*
      @ createNew
      Arguments:
        * L - Lua State
        T*	- Instance to push

      Description:
        Loads an instance of the class into the Lua stack, and provides you a pointer so you can modify it.
    */
    static void push(lua_State* L, T* instance) {
        T** a = (T**)lua_newuserdata(L, sizeof(T*));  // Create userdata
        *a = instance;

        luaL_getmetatable(L, T::Binding.GetName());

        lua_setmetatable(L, -2);
    }

    /*
      @ property_getter (internal)
      Arguments:
        * L - Lua State
    */
    static int property_getter(lua_State* L) {
        lua_getmetatable(L, 1);  // Look up the index of a name
        lua_pushvalue(L, 2);     // Push the name
        lua_rawget(L, -2);       // Get the index

        if (lua_isnumber(L, -1)) {  // Check if we got a valid index
            unsigned _index = static_cast<unsigned>(lua_tonumber(L, -1));

            T** obj = static_cast<T**>(lua_touserdata(L, 1));

            lua_pushvalue(L, 3);

            if (_index & (1u << 8u))  // A func
            {
                lua_pushnumber(L, _index ^ (1u << 8u));  // Push the right func index
                lua_pushlightuserdata(L, obj);
                lua_pushcclosure(L, &LuaClass<T>::function_dispatch, 2);
                return 1;  // Return a func
            }

            lua_pop(L, 2);     // Pop metatable and _index
            lua_remove(L, 1);  // Remove userdata
            lua_remove(L, 1);  // Remove [key]

            return ((*obj)->*(T::Binding.GetProperties()[_index].getter))(L);
        }

        return 1;
    }

    /*
      @ property_setter (internal)
      Arguments:
        * L - Lua State
    */
    static int property_setter(lua_State* L) {
        lua_getmetatable(L, 1);  // Look up the index from name
        lua_pushvalue(L, 2);     //
        lua_rawget(L, -2);       //

        if (lua_isnumber(L, -1)) {  // Check if we got a valid index
            unsigned _index = static_cast<unsigned>(lua_tonumber(L, -1));

            T** obj = static_cast<T**>(lua_touserdata(L, 1));

            if (!obj || !*obj) {
                luaL_error(L, "Internal error, no object given!");
                return 0;
            }

            if (_index >> 8u) {  // Try to set a func
                char c[128];
                sprintf(c, "Trying to set the method [%s] of class [%s]",
                        (*obj)->T::Binding.GetMethods()[_index ^ (1u << 8u)].name, T::Binding.GetName());
                luaL_error(L, c);
                return 0;
            }

            lua_pop(L, 2);     // Pop metatable and _index
            lua_remove(L, 1);  // Remove userdata
            lua_remove(L, 1);  // Remove [key]

            return ((*obj)->*(T::Binding.GetProperties()[_index].setter))(L);
        }

        return 0;
    }

    /*
      @ function_dispatch (internal)
      Arguments:
        * L - Lua State
    */
    static int function_dispatch(lua_State* L) {
        int i = (int)lua_tonumber(L, lua_upvalueindex(1));
        T** obj = static_cast<T**>(lua_touserdata(L, lua_upvalueindex(2)));

        return ((*obj)->*(T::Binding.GetMethods()[i].func))(L);
    }

    /*
      @ gc_obj (internal)
      Arguments:
        * L - Lua State
    */
    static int gc_obj(lua_State* L) {
        T** obj = static_cast<T**>(lua_touserdata(L, -1));

        if (obj && *obj) delete (*obj);

        return 0;
    }

    static int to_string(lua_State* L) {
        T** obj = static_cast<T**>(lua_touserdata(L, -1));

        if (obj)
            lua_pushfstring(L, "%s (%p)", T::Binding.GetName(), (void*)*obj);
        else
            lua_pushstring(L, "Empty object");

        return 1;
    }

    /*
     * Method which compares two Luna objects.
     * The full userdatas (as opposed to light userdata) can't be natively compared one to other, we have to had this to
     * do it.
     */
    static int equals(lua_State* L) {
        T** obj1 = static_cast<T**>(lua_touserdata(L, -1));
        T** obj2 = static_cast<T**>(lua_touserdata(L, 1));

        lua_pushboolean(L, *obj1 == *obj2);

        return 1;
    }
};
}  // namespace lua
