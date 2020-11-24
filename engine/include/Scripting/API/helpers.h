#pragma once

#include <moon/moon.h>

#include "Events/event.h"
#include "Input/input.h"
#include "Input/keycodes.h"

namespace lua {
class Helpers {
public:
    static void Register() {
        RegisterKeyCodes();
        RegisterEventTypes();
        Moon::RegisterFunction("IsKeyPressed", IsKeyPressed);
        Moon::RegisterFunction("RandomNumber", RandomNumber);
        Moon::RegisterFunction("GenerateRandomSeed", GenerateRandomSeed);
        Moon::RegisterFunction("trace", trace);
        Moon::RegisterFunction("warn", warn);
        Moon::RegisterFunction("error", error);
    }

    static void RegisterKeyCodes() {
        Moon::PushGlobalVariable("KEY_UNKNOWN", DE_KEY_UNKNOWN);
        Moon::PushGlobalVariable("KEY_SPACE", DE_KEY_SPACE);
        Moon::PushGlobalVariable("KEY_APOSTROPHE", DE_KEY_APOSTROPHE);
        Moon::PushGlobalVariable("KEY_COMMA", DE_KEY_COMMA);
        Moon::PushGlobalVariable("KEY_MINUS", DE_KEY_MINUS);
        Moon::PushGlobalVariable("KEY_PERIOD", DE_KEY_PERIOD);
        Moon::PushGlobalVariable("KEY_SLASH", DE_KEY_SLASH);
        Moon::PushGlobalVariable("KEY_0", DE_KEY_0);
        Moon::PushGlobalVariable("KEY_1", DE_KEY_1);
        Moon::PushGlobalVariable("KEY_2", DE_KEY_2);
        Moon::PushGlobalVariable("KEY_3", DE_KEY_3);
        Moon::PushGlobalVariable("KEY_4", DE_KEY_4);
        Moon::PushGlobalVariable("KEY_5", DE_KEY_5);
        Moon::PushGlobalVariable("KEY_6", DE_KEY_6);
        Moon::PushGlobalVariable("KEY_7", DE_KEY_7);
        Moon::PushGlobalVariable("KEY_8", DE_KEY_8);
        Moon::PushGlobalVariable("KEY_9", DE_KEY_9);
        Moon::PushGlobalVariable("KEY_SEMICOLON", DE_KEY_SEMICOLON);
        Moon::PushGlobalVariable("KEY_EQUAL", DE_KEY_EQUAL);
        Moon::PushGlobalVariable("KEY_A", DE_KEY_A);
        Moon::PushGlobalVariable("KEY_B", DE_KEY_B);
        Moon::PushGlobalVariable("KEY_C", DE_KEY_C);
        Moon::PushGlobalVariable("KEY_D", DE_KEY_D);
        Moon::PushGlobalVariable("KEY_E", DE_KEY_E);
        Moon::PushGlobalVariable("KEY_F", DE_KEY_F);
        Moon::PushGlobalVariable("KEY_G", DE_KEY_G);
        Moon::PushGlobalVariable("KEY_H", DE_KEY_H);
        Moon::PushGlobalVariable("KEY_I", DE_KEY_I);
        Moon::PushGlobalVariable("KEY_J", DE_KEY_J);
        Moon::PushGlobalVariable("KEY_K", DE_KEY_K);
        Moon::PushGlobalVariable("KEY_L", DE_KEY_L);
        Moon::PushGlobalVariable("KEY_M", DE_KEY_M);
        Moon::PushGlobalVariable("KEY_N", DE_KEY_N);
        Moon::PushGlobalVariable("KEY_O", DE_KEY_O);
        Moon::PushGlobalVariable("KEY_P", DE_KEY_P);
        Moon::PushGlobalVariable("KEY_Q", DE_KEY_Q);
        Moon::PushGlobalVariable("KEY_R", DE_KEY_R);
        Moon::PushGlobalVariable("KEY_S", DE_KEY_S);
        Moon::PushGlobalVariable("KEY_T", DE_KEY_T);
        Moon::PushGlobalVariable("KEY_U", DE_KEY_U);
        Moon::PushGlobalVariable("KEY_V", DE_KEY_V);
        Moon::PushGlobalVariable("KEY_W", DE_KEY_W);
        Moon::PushGlobalVariable("KEY_X", DE_KEY_X);
        Moon::PushGlobalVariable("KEY_Y", DE_KEY_Y);
        Moon::PushGlobalVariable("KEY_Z", DE_KEY_Z);
        Moon::PushGlobalVariable("KEY_LEFT_BRACKET", DE_KEY_LEFT_BRACKET);
        Moon::PushGlobalVariable("KEY_BACKSLASH", DE_KEY_BACKSLASH);
        Moon::PushGlobalVariable("KEY_RIGHT_BRACKET", DE_KEY_RIGHT_BRACKET);
        Moon::PushGlobalVariable("KEY_GRAVE_ACCENT", DE_KEY_GRAVE_ACCENT);
        Moon::PushGlobalVariable("KEY_WORLD_1", DE_KEY_WORLD_1);
        Moon::PushGlobalVariable("KEY_WORLD_2", DE_KEY_WORLD_2);
        Moon::PushGlobalVariable("KEY_ESCAPE", DE_KEY_ESCAPE);
        Moon::PushGlobalVariable("KEY_ENTER", DE_KEY_ENTER);
        Moon::PushGlobalVariable("KEY_TAB", DE_KEY_TAB);
        Moon::PushGlobalVariable("KEY_BACKSPACE", DE_KEY_BACKSPACE);
        Moon::PushGlobalVariable("KEY_INSERT", DE_KEY_INSERT);
        Moon::PushGlobalVariable("KEY_DELETE", DE_KEY_DELETE);
        Moon::PushGlobalVariable("KEY_RIGHT", DE_KEY_RIGHT);
        Moon::PushGlobalVariable("KEY_LEFT", DE_KEY_LEFT);
        Moon::PushGlobalVariable("KEY_DOWN", DE_KEY_DOWN);
        Moon::PushGlobalVariable("KEY_UP", DE_KEY_UP);
        Moon::PushGlobalVariable("KEY_PAGE_UP", DE_KEY_PAGE_UP);
        Moon::PushGlobalVariable("KEY_PAGE_DOWN", DE_KEY_PAGE_DOWN);
        Moon::PushGlobalVariable("KEY_HOME", DE_KEY_HOME);
        Moon::PushGlobalVariable("KEY_END", DE_KEY_END);
        Moon::PushGlobalVariable("KEY_CAPS_LOCK", DE_KEY_CAPS_LOCK);
        Moon::PushGlobalVariable("KEY_SCROLL_LOCK", DE_KEY_SCROLL_LOCK);
        Moon::PushGlobalVariable("KEY_NUM_LOCK", DE_KEY_NUM_LOCK);
        Moon::PushGlobalVariable("KEY_PRINT_SCREEN", DE_KEY_PRINT_SCREEN);
        Moon::PushGlobalVariable("KEY_PAUSE", DE_KEY_PAUSE);
        Moon::PushGlobalVariable("KEY_F1", DE_KEY_F1);
        Moon::PushGlobalVariable("KEY_F2", DE_KEY_F2);
        Moon::PushGlobalVariable("KEY_F3", DE_KEY_F3);
        Moon::PushGlobalVariable("KEY_F4", DE_KEY_F4);
        Moon::PushGlobalVariable("KEY_F5", DE_KEY_F5);
        Moon::PushGlobalVariable("KEY_F6", DE_KEY_F6);
        Moon::PushGlobalVariable("KEY_F7", DE_KEY_F7);
        Moon::PushGlobalVariable("KEY_F8", DE_KEY_F8);
        Moon::PushGlobalVariable("KEY_F9", DE_KEY_F9);
        Moon::PushGlobalVariable("KEY_F10", DE_KEY_F10);
        Moon::PushGlobalVariable("KEY_F11", DE_KEY_F11);
        Moon::PushGlobalVariable("KEY_F12", DE_KEY_F12);
        Moon::PushGlobalVariable("KEY_F13", DE_KEY_F13);
        Moon::PushGlobalVariable("KEY_F14", DE_KEY_F14);
        Moon::PushGlobalVariable("KEY_F15", DE_KEY_F15);
        Moon::PushGlobalVariable("KEY_F16", DE_KEY_F16);
        Moon::PushGlobalVariable("KEY_F17", DE_KEY_F17);
        Moon::PushGlobalVariable("KEY_F18", DE_KEY_F18);
        Moon::PushGlobalVariable("KEY_F19", DE_KEY_F19);
        Moon::PushGlobalVariable("KEY_F20", DE_KEY_F20);
        Moon::PushGlobalVariable("KEY_F21", DE_KEY_F21);
        Moon::PushGlobalVariable("KEY_F22", DE_KEY_F22);
        Moon::PushGlobalVariable("KEY_F23", DE_KEY_F23);
        Moon::PushGlobalVariable("KEY_F24", DE_KEY_F24);
        Moon::PushGlobalVariable("KEY_F25", DE_KEY_F25);
        Moon::PushGlobalVariable("KEY_KP_0", DE_KEY_KP_0);
        Moon::PushGlobalVariable("KEY_KP_1", DE_KEY_KP_1);
        Moon::PushGlobalVariable("KEY_KP_2", DE_KEY_KP_2);
        Moon::PushGlobalVariable("KEY_KP_3", DE_KEY_KP_3);
        Moon::PushGlobalVariable("KEY_KP_4", DE_KEY_KP_4);
        Moon::PushGlobalVariable("KEY_KP_5", DE_KEY_KP_5);
        Moon::PushGlobalVariable("KEY_KP_6", DE_KEY_KP_6);
        Moon::PushGlobalVariable("KEY_KP_7", DE_KEY_KP_7);
        Moon::PushGlobalVariable("KEY_KP_8", DE_KEY_KP_8);
        Moon::PushGlobalVariable("KEY_KP_9", DE_KEY_KP_9);
        Moon::PushGlobalVariable("KEY_KP_DECIMAL", DE_KEY_KP_DECIMAL);
        Moon::PushGlobalVariable("KEY_KP_DIVIDE", DE_KEY_KP_DIVIDE);
        Moon::PushGlobalVariable("KEY_KP_MULTIPLY", DE_KEY_KP_MULTIPLY);
        Moon::PushGlobalVariable("KEY_KP_SUBTRACT", DE_KEY_KP_SUBTRACT);
        Moon::PushGlobalVariable("KEY_KP_ADD", DE_KEY_KP_ADD);
        Moon::PushGlobalVariable("KEY_KP_ENTER", DE_KEY_KP_ENTER);
        Moon::PushGlobalVariable("KEY_KP_EQUAL", DE_KEY_KP_EQUAL);
        Moon::PushGlobalVariable("KEY_LEFT_SHIFT", DE_KEY_LEFT_SHIFT);
        Moon::PushGlobalVariable("KEY_LEFT_CONTROL", DE_KEY_LEFT_CONTROL);
        Moon::PushGlobalVariable("KEY_LEFT_ALT", DE_KEY_LEFT_ALT);
        Moon::PushGlobalVariable("KEY_LEFT_SUPER", DE_KEY_LEFT_SUPER);
        Moon::PushGlobalVariable("KEY_RIGHT_SHIFT", DE_KEY_RIGHT_SHIFT);
        Moon::PushGlobalVariable("KEY_RIGHT_CONTROL", DE_KEY_RIGHT_CONTROL);
        Moon::PushGlobalVariable("KEY_RIGHT_ALT", DE_KEY_RIGHT_ALT);
        Moon::PushGlobalVariable("KEY_RIGHT_SUPER", DE_KEY_RIGHT_SUPER);
        Moon::PushGlobalVariable("KEY_MENU", DE_KEY_MENU);
    }

    static void RegisterEventTypes() {
        Moon::PushGlobalVariable("EVT_NONE", static_cast<int>(de::EventType::None));
        Moon::PushGlobalVariable("EVT_WINDOW_CLOSE", static_cast<int>(de::EventType::WindowClose));
        Moon::PushGlobalVariable("EVT_WINDOW_RESIZE", static_cast<int>(de::EventType::WindowResize));
        Moon::PushGlobalVariable("EVT_WINDOW_ICONIFY", static_cast<int>(de::EventType::WindowIconify));
        Moon::PushGlobalVariable("EVT_WINDOW_FOCUS", static_cast<int>(de::EventType::WindowFocus));
        Moon::PushGlobalVariable("EVT_WINDOW_LOST_FOCUS", static_cast<int>(de::EventType::WindowLostFocus));
        Moon::PushGlobalVariable("EVT_WINDOW_MOVED", static_cast<int>(de::EventType::WindowMoved));
        Moon::PushGlobalVariable("EVT_APP_TICK", static_cast<int>(de::EventType::AppTick));
        Moon::PushGlobalVariable("EVT_APP_UPDATE", static_cast<int>(de::EventType::AppUpdate));
        Moon::PushGlobalVariable("EVT_APP_RENDER", static_cast<int>(de::EventType::AppRender));
        Moon::PushGlobalVariable("EVT_KEY_PRESSED", static_cast<int>(de::EventType::KeyPressed));
        Moon::PushGlobalVariable("EVT_KEY_RELEASE", static_cast<int>(de::EventType::KeyReleased));
        Moon::PushGlobalVariable("EVT_KEY_TYPED", static_cast<int>(de::EventType::KeyTyped));
        Moon::PushGlobalVariable("EVT_MOUSE_BTN_PRESSED", static_cast<int>(de::EventType::MouseBtnPressed));
        Moon::PushGlobalVariable("EVT_MOUSE_BTN_RELEASED", static_cast<int>(de::EventType::MouseBtnReleased));
        Moon::PushGlobalVariable("EVT_MOUSE_MOVED", static_cast<int>(de::EventType::MouseMoved));
        Moon::PushGlobalVariable("EVT_MOUSE_SCROLLED", static_cast<int>(de::EventType::MouseScrolled));
    }

    static MOON_METHOD(IsKeyPressed) {
        Moon::PushValue(de::Input::IsKeyPressed(Moon::GetValue<int>()));
        return 1;
    }

    static MOON_METHOD(GenerateRandomSeed) {
        static bool seed = false;
        if (!seed) {
            std::srand(std::time(nullptr));
            seed = true;
        }
        return 0;
    }

    static MOON_METHOD(RandomNumber) {
        const auto low = Moon::GetValue<int>();
        const auto high = Moon::GetValue<int>(2);
        if (low > high) {
            Moon::PushValue(high);
            return 1;
        }
        Moon::PushValue(low + (std::rand() % (high - low + 1)));
        return 1;
    }

    static MOON_METHOD(trace) {
        LOG_TRACE(Moon::GetValue<std::string>());
        return 0;
    }

    static MOON_METHOD(warn) {
        LOG_WARN(Moon::GetValue<std::string>());
        return 0;
    }

    static MOON_METHOD(error) {
        LOG_ERROR(Moon::GetValue<std::string>());
        return 0;
    }
};
}  // namespace lua
