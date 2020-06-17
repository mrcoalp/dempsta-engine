#pragma once

#include <vector>

#include "Scripting/luaclass.h"

namespace lua {
template <typename T>
class Binding {
public:
    explicit Binding(const char* name) : m_name(name) {}

    ~Binding() = default;

    [[nodiscard]] inline const char* GetName() const { return m_name; }

    [[nodiscard]] inline const auto& GetMethods() const { return m_methods; }

    [[nodiscard]] inline const auto& GetProperties() const { return m_properties; }

    Binding& AddMethod(typename LuaClass<T>::FunctionType func) {
        m_methods.push_back(func);
        return *this;
    }

    Binding& AddProperty(typename LuaClass<T>::PropertyType prop) {
        m_properties.push_back(prop);
        return *this;
    }

private:
    const char* m_name;
    std::vector<typename LuaClass<T>::FunctionType> m_methods;
    std::vector<typename LuaClass<T>::PropertyType> m_properties;
};
}  // namespace lua

#define LUA_DECLARE_CLASS(_class) \
    using BindingType = _class;   \
    static lua::Binding<_class> Binding;
// TODO(MPINTO): Change to support all data types and not only primitives
// NOTE(MPINTO): Marshalling is needed when using this macro
#define LUA_PROPERTY(_property, _type)                        \
    int Get_##_property(lua_State* L) {                       \
        MS::PushValue(L, _property);                         \
        return 1;                                             \
    }                                                         \
    int Set_##_property(lua_State* L) {                       \
        _property = MS::GetValue(lua::Type<_type>{}, L, -1); \
        return 0;                                             \
    }
#define LUA_PROXY_METHOD(_name) int _name(lua_State* L)
#define LUA_DEFINE_BINDING(_class) lua::Binding<_class> _class::Binding = lua::Binding<_class>(#_class)
#define LUA_ADD_METHOD(_method) .AddMethod({#_method, &BindingType::_method})
#define LUA_ADD_PROPERTY(_prop) .AddProperty({#_prop, &BindingType::Get_##_prop, &BindingType::Set_##_prop})
