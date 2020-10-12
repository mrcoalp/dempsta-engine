#pragma once

#include <string>

struct ALCdevice;

namespace de {
class OpenALUtils {
public:
    template <typename alFunction, typename... Params>
    static auto ALCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params) ->
        typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
        auto ret = function(std::forward<Params>(params)...);
        check_al_errors(filename, line);
        return ret;
    }

    template <typename alFunction, typename... Params>
    static auto ALCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params) ->
        typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
        function(std::forward<Params>(params)...);
        return check_al_errors(filename, line);
    }

    template <typename alcFunction, typename... Params>
    static auto ALCCallImpl(const char* filename, const std::uint_fast32_t line, alcFunction function, ALCdevice* device, Params... params) ->
        typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
        function(std::forward<Params>(params)...);
        return check_alc_errors(filename, line, device);
    }

    template <typename alcFunction, typename ReturnType, typename... Params>
    static auto ALCCallImpl(const char* filename, const std::uint_fast32_t line, alcFunction function, ReturnType& returnValue, ALCdevice* device,
                            Params... params) -> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool> {
        returnValue = function(std::forward<Params>(params)...);
        return check_alc_errors(filename, line, device);
    }

private:
    static bool check_al_errors(const std::string& filename, std::uint_fast32_t line);
    static bool check_alc_errors(const std::string& filename, std::uint_fast32_t line, ALCdevice* device);
};

#ifdef NDEBUG
#define AL_CALL(_function, ...) ::de::OpenALUtils::ALCallImpl(__FILE__, __LINE__, _function, __VA_ARGS__)
#define ALC_CALL(function, device, ...) ::de::OpenALUtils::ALCCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)
#else
#define AL_CALL(_function, ...)
#define ALC_CALL(function, device, ...)
#endif
}  // namespace de
