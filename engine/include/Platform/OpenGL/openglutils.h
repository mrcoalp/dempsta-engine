#pragma once

#include <string>

namespace de {
class OpenGLUtils {
public:
    template <typename GLFunction, typename... Args>
    static auto GLCallImpl(const char* filename, const std::uint_fast32_t line, GLFunction function, Args... args) ->
        typename std::enable_if_t<!std::is_same_v<void, decltype(function(args...))>, decltype(function(args...))> {
        auto ret = function(std::forward<Args>(args)...);
        check_gl_errors(filename, line);
        return ret;
    }

    template <typename GLFunction, typename... Args>
    static auto GLCallImpl(const char* filename, const std::uint_fast32_t line, GLFunction function, Args... args) ->
        typename std::enable_if_t<std::is_same_v<void, decltype(function(args...))>, bool> {
        function(std::forward<Args>(args)...);
        return check_gl_errors(filename, line);
    }

private:
    static bool check_gl_errors(const std::string& filename, std::uint_fast32_t line);
};

#define GL_CALL(_function, ...) ::de::OpenGLUtils::GLCallImpl(__FILE__, __LINE__, _function, __VA_ARGS__)
#define GL_CALL_NO_ARGS(_function) ::de::OpenGLUtils::GLCallImpl(__FILE__, __LINE__, _function)
}  // namespace de
