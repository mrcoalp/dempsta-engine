#include <Core/log.h>

#include <map>

#include "json_test.h"
#include "scripting.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

using Test = std::function<bool()>;
static std::map<const char*, Test> tests = {
    {"test_call_cpp_function_from_lua", test_call_cpp_function_from_lua},
    {"test_call_lua_function_pass_binding_object_modify_inside_lua", test_call_lua_function_pass_binding_object_modify_inside_lua},
    {"test_call_lua_function_pass_vector_get_string", test_call_lua_function_pass_vector_get_string},
    {"test_call_lua_function_pass_multiple_params_get_int", test_call_lua_function_pass_multiple_params_get_int},
    {"test_call_lua_function_pass_multiple_params_get_vector", test_call_lua_function_pass_multiple_params_get_vector},
    {"test_call_lua_function_get_anonymous_function_and_call_it", test_call_lua_function_get_anonymous_function_and_call_it},
    {"test_cpp_class_bind_lua", test_cpp_class_bind_lua},
    {"test_get_global_lua_var_from_cpp", test_get_global_lua_var_from_cpp},
    {"test_lua_run_code", test_lua_run_code},
    {"test_lua_data_buffer", test_lua_data_buffer},
    {"test_get_dynamic_map_from_lua", test_get_dynamic_map_from_lua},
    {"test_json_parse_simple_string_struct", test_json_parse_simple_string_struct},
    {"test_json_parse_errors", test_json_parse_errors},
    {"test_json_parse_array", test_json_parse_array},
    {"test_json_parse_map", test_json_parse_map},
    {"test_json_parse_complex_struct", test_json_parse_complex_struct}};

int RunTests() {
    const int nrOfTests = (int)tests.size();
    int passed = 0;
    printf(ANSI_COLOR_CYAN "Executing %d tests..." ANSI_COLOR_RESET "\n", nrOfTests);
    for (const auto& test : tests) {
        try {
            if (test.second()) {
                ++passed;
                printf(ANSI_COLOR_GREEN "'%s' - passed" ANSI_COLOR_RESET "\n", test.first);
            } else {
                printf(ANSI_COLOR_RED "'%s' - failed" ANSI_COLOR_RESET "\n", test.first);
            }
        } catch (const std::exception& e) {
            printf(ANSI_COLOR_RED "'%s' raised an exception: %s" ANSI_COLOR_RESET "\n", test.first, e.what());
        } catch (...) {
            printf(ANSI_COLOR_RED "'%s' raised an unknown exception" ANSI_COLOR_RESET "\n", test.first);
        }
    }
    const int result = nrOfTests - passed;
    if (result == 0) {
        printf("\n" ANSI_COLOR_GREEN "ALL TESTS PASSED!" ANSI_COLOR_RESET "\n");
    } else {
        printf("\n" ANSI_COLOR_RED "%d TESTS FAILED!" ANSI_COLOR_RESET "\n", result);
    }
    return result;
}

int main(int argc, char** argv) { return RunTests(); }
