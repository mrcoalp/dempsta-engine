#include <map>

#include "scripting.h"

using Test = std::function<bool()>;
static std::map<const char*, Test> tests = {{"test_call_cpp_function_from_lua", test_call_cpp_function_from_lua},
                                            {"test_call_lua_function_from_cpp", test_call_lua_function_from_cpp},
                                            {"test_cpp_class_bind_lua", test_cpp_class_bind_lua},
                                            {"test_get_global_lua_var_from_cpp", test_get_global_lua_var_from_cpp}};

int RunTests() {
    const int nrOfTests = tests.size();
    int passed = 0;
    LOG_DEBUG("Executing {} tests...", nrOfTests);
    for (const auto& test : tests) {
        try {
            if (test.second()) {
                ++passed;
                LOG_TRACE("'{}' passed", test.first);
            } else {
                LOG_ERROR("'{}' failed", test.first);
            }
        } catch (std::exception& e) {
            LOG_ERROR("'{}' raised an exception: {}", test.first, e.what());
        } catch (...) {
            LOG_ERROR("'{}' raised an unknown exception", test.first);
        }
    }
    const int result = nrOfTests - passed;
    if (result == 0) {
        LOG_INFO("All tests passed!");
    } else {
        LOG_CRITICAL("{} tests failed!", result);
    }
    return result;
}

int main(int argc, char** argv) {
    de::Log::Init();
    return RunTests();
}
