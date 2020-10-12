#pragma once

#include "Events/event.h"
#include "Input/input.h"
#include "Input/keycodes.h"
#include "Scripting/luaengine.h"

namespace lua {
class Helpers {
public:
    static void Register() {
        RegisterKeyCodes();
        RegisterEventTypes();
        LE::RegisterFunction("IsKeyPressed", IsKeyPressed);
        LE::RegisterFunction("RandomNumber", RandomNumber);
        LE::RegisterFunction("GenerateRandomSeed", GenerateRandomSeed);
        LE::RegisterFunction("trace", trace);
        LE::RegisterFunction("warn", warn);
        LE::RegisterFunction("error", error);
    }

    static void RegisterKeyCodes() {
        LE::PushGlobalVariable("KEY_UNKNOWN", DE_KEY_UNKNOWN);
        LE::PushGlobalVariable("KEY_SPACE", DE_KEY_SPACE);
        LE::PushGlobalVariable("KEY_APOSTROPHE", DE_KEY_APOSTROPHE);
        LE::PushGlobalVariable("KEY_COMMA", DE_KEY_COMMA);
        LE::PushGlobalVariable("KEY_MINUS", DE_KEY_MINUS);
        LE::PushGlobalVariable("KEY_PERIOD", DE_KEY_PERIOD);
        LE::PushGlobalVariable("KEY_SLASH", DE_KEY_SLASH);
        LE::PushGlobalVariable("KEY_0", DE_KEY_0);
        LE::PushGlobalVariable("KEY_1", DE_KEY_1);
        LE::PushGlobalVariable("KEY_2", DE_KEY_2);
        LE::PushGlobalVariable("KEY_3", DE_KEY_3);
        LE::PushGlobalVariable("KEY_4", DE_KEY_4);
        LE::PushGlobalVariable("KEY_5", DE_KEY_5);
        LE::PushGlobalVariable("KEY_6", DE_KEY_6);
        LE::PushGlobalVariable("KEY_7", DE_KEY_7);
        LE::PushGlobalVariable("KEY_8", DE_KEY_8);
        LE::PushGlobalVariable("KEY_9", DE_KEY_9);
        LE::PushGlobalVariable("KEY_SEMICOLON", DE_KEY_SEMICOLON);
        LE::PushGlobalVariable("KEY_EQUAL", DE_KEY_EQUAL);
        LE::PushGlobalVariable("KEY_A", DE_KEY_A);
        LE::PushGlobalVariable("KEY_B", DE_KEY_B);
        LE::PushGlobalVariable("KEY_C", DE_KEY_C);
        LE::PushGlobalVariable("KEY_D", DE_KEY_D);
        LE::PushGlobalVariable("KEY_E", DE_KEY_E);
        LE::PushGlobalVariable("KEY_F", DE_KEY_F);
        LE::PushGlobalVariable("KEY_G", DE_KEY_G);
        LE::PushGlobalVariable("KEY_H", DE_KEY_H);
        LE::PushGlobalVariable("KEY_I", DE_KEY_I);
        LE::PushGlobalVariable("KEY_J", DE_KEY_J);
        LE::PushGlobalVariable("KEY_K", DE_KEY_K);
        LE::PushGlobalVariable("KEY_L", DE_KEY_L);
        LE::PushGlobalVariable("KEY_M", DE_KEY_M);
        LE::PushGlobalVariable("KEY_N", DE_KEY_N);
        LE::PushGlobalVariable("KEY_O", DE_KEY_O);
        LE::PushGlobalVariable("KEY_P", DE_KEY_P);
        LE::PushGlobalVariable("KEY_Q", DE_KEY_Q);
        LE::PushGlobalVariable("KEY_R", DE_KEY_R);
        LE::PushGlobalVariable("KEY_S", DE_KEY_S);
        LE::PushGlobalVariable("KEY_T", DE_KEY_T);
        LE::PushGlobalVariable("KEY_U", DE_KEY_U);
        LE::PushGlobalVariable("KEY_V", DE_KEY_V);
        LE::PushGlobalVariable("KEY_W", DE_KEY_W);
        LE::PushGlobalVariable("KEY_X", DE_KEY_X);
        LE::PushGlobalVariable("KEY_Y", DE_KEY_Y);
        LE::PushGlobalVariable("KEY_Z", DE_KEY_Z);
        LE::PushGlobalVariable("KEY_LEFT_BRACKET", DE_KEY_LEFT_BRACKET);
        LE::PushGlobalVariable("KEY_BACKSLASH", DE_KEY_BACKSLASH);
        LE::PushGlobalVariable("KEY_RIGHT_BRACKET", DE_KEY_RIGHT_BRACKET);
        LE::PushGlobalVariable("KEY_GRAVE_ACCENT", DE_KEY_GRAVE_ACCENT);
        LE::PushGlobalVariable("KEY_WORLD_1", DE_KEY_WORLD_1);
        LE::PushGlobalVariable("KEY_WORLD_2", DE_KEY_WORLD_2);
        LE::PushGlobalVariable("KEY_ESCAPE", DE_KEY_ESCAPE);
        LE::PushGlobalVariable("KEY_ENTER", DE_KEY_ENTER);
        LE::PushGlobalVariable("KEY_TAB", DE_KEY_TAB);
        LE::PushGlobalVariable("KEY_BACKSPACE", DE_KEY_BACKSPACE);
        LE::PushGlobalVariable("KEY_INSERT", DE_KEY_INSERT);
        LE::PushGlobalVariable("KEY_DELETE", DE_KEY_DELETE);
        LE::PushGlobalVariable("KEY_RIGHT", DE_KEY_RIGHT);
        LE::PushGlobalVariable("KEY_LEFT", DE_KEY_LEFT);
        LE::PushGlobalVariable("KEY_DOWN", DE_KEY_DOWN);
        LE::PushGlobalVariable("KEY_UP", DE_KEY_UP);
        LE::PushGlobalVariable("KEY_PAGE_UP", DE_KEY_PAGE_UP);
        LE::PushGlobalVariable("KEY_PAGE_DOWN", DE_KEY_PAGE_DOWN);
        LE::PushGlobalVariable("KEY_HOME", DE_KEY_HOME);
        LE::PushGlobalVariable("KEY_END", DE_KEY_END);
        LE::PushGlobalVariable("KEY_CAPS_LOCK", DE_KEY_CAPS_LOCK);
        LE::PushGlobalVariable("KEY_SCROLL_LOCK", DE_KEY_SCROLL_LOCK);
        LE::PushGlobalVariable("KEY_NUM_LOCK", DE_KEY_NUM_LOCK);
        LE::PushGlobalVariable("KEY_PRINT_SCREEN", DE_KEY_PRINT_SCREEN);
        LE::PushGlobalVariable("KEY_PAUSE", DE_KEY_PAUSE);
        LE::PushGlobalVariable("KEY_F1", DE_KEY_F1);
        LE::PushGlobalVariable("KEY_F2", DE_KEY_F2);
        LE::PushGlobalVariable("KEY_F3", DE_KEY_F3);
        LE::PushGlobalVariable("KEY_F4", DE_KEY_F4);
        LE::PushGlobalVariable("KEY_F5", DE_KEY_F5);
        LE::PushGlobalVariable("KEY_F6", DE_KEY_F6);
        LE::PushGlobalVariable("KEY_F7", DE_KEY_F7);
        LE::PushGlobalVariable("KEY_F8", DE_KEY_F8);
        LE::PushGlobalVariable("KEY_F9", DE_KEY_F9);
        LE::PushGlobalVariable("KEY_F10", DE_KEY_F10);
        LE::PushGlobalVariable("KEY_F11", DE_KEY_F11);
        LE::PushGlobalVariable("KEY_F12", DE_KEY_F12);
        LE::PushGlobalVariable("KEY_F13", DE_KEY_F13);
        LE::PushGlobalVariable("KEY_F14", DE_KEY_F14);
        LE::PushGlobalVariable("KEY_F15", DE_KEY_F15);
        LE::PushGlobalVariable("KEY_F16", DE_KEY_F16);
        LE::PushGlobalVariable("KEY_F17", DE_KEY_F17);
        LE::PushGlobalVariable("KEY_F18", DE_KEY_F18);
        LE::PushGlobalVariable("KEY_F19", DE_KEY_F19);
        LE::PushGlobalVariable("KEY_F20", DE_KEY_F20);
        LE::PushGlobalVariable("KEY_F21", DE_KEY_F21);
        LE::PushGlobalVariable("KEY_F22", DE_KEY_F22);
        LE::PushGlobalVariable("KEY_F23", DE_KEY_F23);
        LE::PushGlobalVariable("KEY_F24", DE_KEY_F24);
        LE::PushGlobalVariable("KEY_F25", DE_KEY_F25);
        LE::PushGlobalVariable("KEY_KP_0", DE_KEY_KP_0);
        LE::PushGlobalVariable("KEY_KP_1", DE_KEY_KP_1);
        LE::PushGlobalVariable("KEY_KP_2", DE_KEY_KP_2);
        LE::PushGlobalVariable("KEY_KP_3", DE_KEY_KP_3);
        LE::PushGlobalVariable("KEY_KP_4", DE_KEY_KP_4);
        LE::PushGlobalVariable("KEY_KP_5", DE_KEY_KP_5);
        LE::PushGlobalVariable("KEY_KP_6", DE_KEY_KP_6);
        LE::PushGlobalVariable("KEY_KP_7", DE_KEY_KP_7);
        LE::PushGlobalVariable("KEY_KP_8", DE_KEY_KP_8);
        LE::PushGlobalVariable("KEY_KP_9", DE_KEY_KP_9);
        LE::PushGlobalVariable("KEY_KP_DECIMAL", DE_KEY_KP_DECIMAL);
        LE::PushGlobalVariable("KEY_KP_DIVIDE", DE_KEY_KP_DIVIDE);
        LE::PushGlobalVariable("KEY_KP_MULTIPLY", DE_KEY_KP_MULTIPLY);
        LE::PushGlobalVariable("KEY_KP_SUBTRACT", DE_KEY_KP_SUBTRACT);
        LE::PushGlobalVariable("KEY_KP_ADD", DE_KEY_KP_ADD);
        LE::PushGlobalVariable("KEY_KP_ENTER", DE_KEY_KP_ENTER);
        LE::PushGlobalVariable("KEY_KP_EQUAL", DE_KEY_KP_EQUAL);
        LE::PushGlobalVariable("KEY_LEFT_SHIFT", DE_KEY_LEFT_SHIFT);
        LE::PushGlobalVariable("KEY_LEFT_CONTROL", DE_KEY_LEFT_CONTROL);
        LE::PushGlobalVariable("KEY_LEFT_ALT", DE_KEY_LEFT_ALT);
        LE::PushGlobalVariable("KEY_LEFT_SUPER", DE_KEY_LEFT_SUPER);
        LE::PushGlobalVariable("KEY_RIGHT_SHIFT", DE_KEY_RIGHT_SHIFT);
        LE::PushGlobalVariable("KEY_RIGHT_CONTROL", DE_KEY_RIGHT_CONTROL);
        LE::PushGlobalVariable("KEY_RIGHT_ALT", DE_KEY_RIGHT_ALT);
        LE::PushGlobalVariable("KEY_RIGHT_SUPER", DE_KEY_RIGHT_SUPER);
        LE::PushGlobalVariable("KEY_MENU", DE_KEY_MENU);
    }

    static void RegisterEventTypes() {
        LE::PushGlobalVariable("EVT_NONE", static_cast<int>(de::EventType::None));
        LE::PushGlobalVariable("EVT_WINDOW_CLOSE", static_cast<int>(de::EventType::WindowClose));
        LE::PushGlobalVariable("EVT_WINDOW_RESIZE", static_cast<int>(de::EventType::WindowResize));
        LE::PushGlobalVariable("EVT_WINDOW_ICONIFY", static_cast<int>(de::EventType::WindowIconify));
        LE::PushGlobalVariable("EVT_WINDOW_FOCUS", static_cast<int>(de::EventType::WindowFocus));
        LE::PushGlobalVariable("EVT_WINDOW_LOST_FOCUS", static_cast<int>(de::EventType::WindowLostFocus));
        LE::PushGlobalVariable("EVT_WINDOW_MOVED", static_cast<int>(de::EventType::WindowMoved));
        LE::PushGlobalVariable("EVT_APP_TICK", static_cast<int>(de::EventType::AppTick));
        LE::PushGlobalVariable("EVT_APP_UPDATE", static_cast<int>(de::EventType::AppUpdate));
        LE::PushGlobalVariable("EVT_APP_RENDER", static_cast<int>(de::EventType::AppRender));
        LE::PushGlobalVariable("EVT_KEY_PRESSED", static_cast<int>(de::EventType::KeyPressed));
        LE::PushGlobalVariable("EVT_KEY_RELEASE", static_cast<int>(de::EventType::KeyReleased));
        LE::PushGlobalVariable("EVT_KEY_TYPED", static_cast<int>(de::EventType::KeyTyped));
        LE::PushGlobalVariable("EVT_MOUSE_BTN_PRESSED", static_cast<int>(de::EventType::MouseBtnPressed));
        LE::PushGlobalVariable("EVT_MOUSE_BTN_RELEASED", static_cast<int>(de::EventType::MouseBtnReleased));
        LE::PushGlobalVariable("EVT_MOUSE_MOVED", static_cast<int>(de::EventType::MouseMoved));
        LE::PushGlobalVariable("EVT_MOUSE_SCROLLED", static_cast<int>(de::EventType::MouseScrolled));
    }

    static LUA_METHOD(IsKeyPressed) {
        LE::PushValue(de::Input::IsKeyPressed(LE::GetValue<int>()));
        return 1;
    }

    static LUA_METHOD(GenerateRandomSeed) {
        static bool seed = false;
        if (!seed) {
            std::srand(std::time(nullptr));
            seed = true;
        }
        return 0;
    }

    static LUA_METHOD(RandomNumber) {
        const auto low = LE::GetValue<int>();
        const auto high = LE::GetValue<int>(2);
        if (low > high) {
            LE::PushValue(high);
            return 1;
        }
        LE::PushValue(low + (std::rand() % (high - low + 1)));
        return 1;
    }

    static LUA_METHOD(trace) {
        LOG_TRACE(LE::GetValue<std::string>());
        return 0;
    }

    static LUA_METHOD(warn) {
        LOG_WARN(LE::GetValue<std::string>());
        return 0;
    }

    static LUA_METHOD(error) {
        LOG_ERROR(LE::GetValue<std::string>());
        return 0;
    }
};
}  // namespace lua
