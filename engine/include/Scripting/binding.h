#pragma once

#include <vector>

#include "Scripting/luaclass.h"

namespace lua {
template <class BindableClass>
class Binding {
public:
    using LuaFunction = typename LuaClass<BindableClass>::FunctionType;
    using LuaProperty = typename LuaClass<BindableClass>::PropertyType;

    explicit Binding(const char* name) : m_name(name) {}

    ~Binding() = default;

    [[nodiscard]] inline const char* GetName() const { return m_name; }

    [[nodiscard]] inline const auto& GetMethods() const { return m_methods; }

    [[nodiscard]] inline const auto& GetProperties() const { return m_properties; }

    Binding& AddMethod(LuaFunction func) {
        m_methods.push_back(func);
        return *this;
    }

    Binding& AddProperty(LuaProperty prop) {
        m_properties.push_back(prop);
        return *this;
    }

private:
    const char* m_name;
    std::vector<LuaFunction> m_methods;
    std::vector<LuaProperty> m_properties;
};
}  // namespace lua

#define LUA_DECLARE_CLASS(_class) \
    static bool GC;               \
    static lua::Binding<_class> Binding;
// TODO(MPINTO): Change to support all data types and not only primitives
// NOTE(MPINTO): Marshalling is needed when using this macro
#define LUA_PROPERTY(_property, _type)     \
    int Get_##_property(lua_State* L) {    \
        LE::PushValue(_property);          \
        return 1;                          \
    }                                      \
    int Set_##_property(lua_State* L) {    \
        _property = LE::GetValue<_type>(); \
        return 0;                          \
    }
#define LUA_METHOD(_name) int _name(lua_State* L)
#define LUA_DEFINE_BINDING(_class, _gc) \
    using BindingType = _class;         \
    bool BindingType::GC = _gc;         \
    lua::Binding<BindingType> BindingType::Binding = lua::Binding<BindingType>(#_class)
#define LUA_ADD_METHOD(_method) .AddMethod({#_method, &BindingType::_method})
#define LUA_ADD_PROPERTY(_prop) .AddProperty({#_prop, &BindingType::Get_##_prop, &BindingType::Set_##_prop})
#define LUA_ADD_PROPERTY_CUSTOM(_prop, _getter, _setter) .AddProperty({#_prop, &BindingType::_getter, &BindingType::_setter})
