#pragma once

#include "Events/event.h"
#include "Input/input.h"
#include "Input/keycodes.h"
#include "Scripting/scriptengine.h"

namespace lua {
class Helpers {
public:
    static void Register() {
        RegisterKeyCodes();
        RegisterEventTypes();
        SE::RegisterFunction("IsKeyPressed", IsKeyPressed);
    }

    static void RegisterKeyCodes() {
        SE::PushGlobalVariable("KEY_UNKNOWN", DE_KEY_UNKNOWN);
        SE::PushGlobalVariable("KEY_SPACE", DE_KEY_SPACE);
        SE::PushGlobalVariable("KEY_APOSTROPHE", DE_KEY_APOSTROPHE);
        SE::PushGlobalVariable("KEY_COMMA", DE_KEY_COMMA);
        SE::PushGlobalVariable("KEY_MINUS", DE_KEY_MINUS);
        SE::PushGlobalVariable("KEY_PERIOD", DE_KEY_PERIOD);
        SE::PushGlobalVariable("KEY_SLASH", DE_KEY_SLASH);
        SE::PushGlobalVariable("KEY_0", DE_KEY_0);
        SE::PushGlobalVariable("KEY_1", DE_KEY_1);
        SE::PushGlobalVariable("KEY_2", DE_KEY_2);
        SE::PushGlobalVariable("KEY_3", DE_KEY_3);
        SE::PushGlobalVariable("KEY_4", DE_KEY_4);
        SE::PushGlobalVariable("KEY_5", DE_KEY_5);
        SE::PushGlobalVariable("KEY_6", DE_KEY_6);
        SE::PushGlobalVariable("KEY_7", DE_KEY_7);
        SE::PushGlobalVariable("KEY_8", DE_KEY_8);
        SE::PushGlobalVariable("KEY_9", DE_KEY_9);
        SE::PushGlobalVariable("KEY_SEMICOLON", DE_KEY_SEMICOLON);
        SE::PushGlobalVariable("KEY_EQUAL", DE_KEY_EQUAL);
        SE::PushGlobalVariable("KEY_A", DE_KEY_A);
        SE::PushGlobalVariable("KEY_B", DE_KEY_B);
        SE::PushGlobalVariable("KEY_C", DE_KEY_C);
        SE::PushGlobalVariable("KEY_D", DE_KEY_D);
        SE::PushGlobalVariable("KEY_E", DE_KEY_E);
        SE::PushGlobalVariable("KEY_F", DE_KEY_F);
        SE::PushGlobalVariable("KEY_G", DE_KEY_G);
        SE::PushGlobalVariable("KEY_H", DE_KEY_H);
        SE::PushGlobalVariable("KEY_I", DE_KEY_I);
        SE::PushGlobalVariable("KEY_J", DE_KEY_J);
        SE::PushGlobalVariable("KEY_K", DE_KEY_K);
        SE::PushGlobalVariable("KEY_L", DE_KEY_L);
        SE::PushGlobalVariable("KEY_M", DE_KEY_M);
        SE::PushGlobalVariable("KEY_N", DE_KEY_N);
        SE::PushGlobalVariable("KEY_O", DE_KEY_O);
        SE::PushGlobalVariable("KEY_P", DE_KEY_P);
        SE::PushGlobalVariable("KEY_Q", DE_KEY_Q);
        SE::PushGlobalVariable("KEY_R", DE_KEY_R);
        SE::PushGlobalVariable("KEY_S", DE_KEY_S);
        SE::PushGlobalVariable("KEY_T", DE_KEY_T);
        SE::PushGlobalVariable("KEY_U", DE_KEY_U);
        SE::PushGlobalVariable("KEY_V", DE_KEY_V);
        SE::PushGlobalVariable("KEY_W", DE_KEY_W);
        SE::PushGlobalVariable("KEY_X", DE_KEY_X);
        SE::PushGlobalVariable("KEY_Y", DE_KEY_Y);
        SE::PushGlobalVariable("KEY_Z", DE_KEY_Z);
        SE::PushGlobalVariable("KEY_LEFT_BRACKET", DE_KEY_LEFT_BRACKET);
        SE::PushGlobalVariable("KEY_BACKSLASH", DE_KEY_BACKSLASH);
        SE::PushGlobalVariable("KEY_RIGHT_BRACKET", DE_KEY_RIGHT_BRACKET);
        SE::PushGlobalVariable("KEY_GRAVE_ACCENT", DE_KEY_GRAVE_ACCENT);
        SE::PushGlobalVariable("KEY_WORLD_1", DE_KEY_WORLD_1);
        SE::PushGlobalVariable("KEY_WORLD_2", DE_KEY_WORLD_2);
        SE::PushGlobalVariable("KEY_ESCAPE", DE_KEY_ESCAPE);
        SE::PushGlobalVariable("KEY_ENTER", DE_KEY_ENTER);
        SE::PushGlobalVariable("KEY_TAB", DE_KEY_TAB);
        SE::PushGlobalVariable("KEY_BACKSPACE", DE_KEY_BACKSPACE);
        SE::PushGlobalVariable("KEY_INSERT", DE_KEY_INSERT);
        SE::PushGlobalVariable("KEY_DELETE", DE_KEY_DELETE);
        SE::PushGlobalVariable("KEY_RIGHT", DE_KEY_RIGHT);
        SE::PushGlobalVariable("KEY_LEFT", DE_KEY_LEFT);
        SE::PushGlobalVariable("KEY_DOWN", DE_KEY_DOWN);
        SE::PushGlobalVariable("KEY_UP", DE_KEY_UP);
        SE::PushGlobalVariable("KEY_PAGE_UP", DE_KEY_PAGE_UP);
        SE::PushGlobalVariable("KEY_PAGE_DOWN", DE_KEY_PAGE_DOWN);
        SE::PushGlobalVariable("KEY_HOME", DE_KEY_HOME);
        SE::PushGlobalVariable("KEY_END", DE_KEY_END);
        SE::PushGlobalVariable("KEY_CAPS_LOCK", DE_KEY_CAPS_LOCK);
        SE::PushGlobalVariable("KEY_SCROLL_LOCK", DE_KEY_SCROLL_LOCK);
        SE::PushGlobalVariable("KEY_NUM_LOCK", DE_KEY_NUM_LOCK);
        SE::PushGlobalVariable("KEY_PRINT_SCREEN", DE_KEY_PRINT_SCREEN);
        SE::PushGlobalVariable("KEY_PAUSE", DE_KEY_PAUSE);
        SE::PushGlobalVariable("KEY_F1", DE_KEY_F1);
        SE::PushGlobalVariable("KEY_F2", DE_KEY_F2);
        SE::PushGlobalVariable("KEY_F3", DE_KEY_F3);
        SE::PushGlobalVariable("KEY_F4", DE_KEY_F4);
        SE::PushGlobalVariable("KEY_F5", DE_KEY_F5);
        SE::PushGlobalVariable("KEY_F6", DE_KEY_F6);
        SE::PushGlobalVariable("KEY_F7", DE_KEY_F7);
        SE::PushGlobalVariable("KEY_F8", DE_KEY_F8);
        SE::PushGlobalVariable("KEY_F9", DE_KEY_F9);
        SE::PushGlobalVariable("KEY_F10", DE_KEY_F10);
        SE::PushGlobalVariable("KEY_F11", DE_KEY_F11);
        SE::PushGlobalVariable("KEY_F12", DE_KEY_F12);
        SE::PushGlobalVariable("KEY_F13", DE_KEY_F13);
        SE::PushGlobalVariable("KEY_F14", DE_KEY_F14);
        SE::PushGlobalVariable("KEY_F15", DE_KEY_F15);
        SE::PushGlobalVariable("KEY_F16", DE_KEY_F16);
        SE::PushGlobalVariable("KEY_F17", DE_KEY_F17);
        SE::PushGlobalVariable("KEY_F18", DE_KEY_F18);
        SE::PushGlobalVariable("KEY_F19", DE_KEY_F19);
        SE::PushGlobalVariable("KEY_F20", DE_KEY_F20);
        SE::PushGlobalVariable("KEY_F21", DE_KEY_F21);
        SE::PushGlobalVariable("KEY_F22", DE_KEY_F22);
        SE::PushGlobalVariable("KEY_F23", DE_KEY_F23);
        SE::PushGlobalVariable("KEY_F24", DE_KEY_F24);
        SE::PushGlobalVariable("KEY_F25", DE_KEY_F25);
        SE::PushGlobalVariable("KEY_KP_0", DE_KEY_KP_0);
        SE::PushGlobalVariable("KEY_KP_1", DE_KEY_KP_1);
        SE::PushGlobalVariable("KEY_KP_2", DE_KEY_KP_2);
        SE::PushGlobalVariable("KEY_KP_3", DE_KEY_KP_3);
        SE::PushGlobalVariable("KEY_KP_4", DE_KEY_KP_4);
        SE::PushGlobalVariable("KEY_KP_5", DE_KEY_KP_5);
        SE::PushGlobalVariable("KEY_KP_6", DE_KEY_KP_6);
        SE::PushGlobalVariable("KEY_KP_7", DE_KEY_KP_7);
        SE::PushGlobalVariable("KEY_KP_8", DE_KEY_KP_8);
        SE::PushGlobalVariable("KEY_KP_9", DE_KEY_KP_9);
        SE::PushGlobalVariable("KEY_KP_DECIMAL", DE_KEY_KP_DECIMAL);
        SE::PushGlobalVariable("KEY_KP_DIVIDE", DE_KEY_KP_DIVIDE);
        SE::PushGlobalVariable("KEY_KP_MULTIPLY", DE_KEY_KP_MULTIPLY);
        SE::PushGlobalVariable("KEY_KP_SUBTRACT", DE_KEY_KP_SUBTRACT);
        SE::PushGlobalVariable("KEY_KP_ADD", DE_KEY_KP_ADD);
        SE::PushGlobalVariable("KEY_KP_ENTER", DE_KEY_KP_ENTER);
        SE::PushGlobalVariable("KEY_KP_EQUAL", DE_KEY_KP_EQUAL);
        SE::PushGlobalVariable("KEY_LEFT_SHIFT", DE_KEY_LEFT_SHIFT);
        SE::PushGlobalVariable("KEY_LEFT_CONTROL", DE_KEY_LEFT_CONTROL);
        SE::PushGlobalVariable("KEY_LEFT_ALT", DE_KEY_LEFT_ALT);
        SE::PushGlobalVariable("KEY_LEFT_SUPER", DE_KEY_LEFT_SUPER);
        SE::PushGlobalVariable("KEY_RIGHT_SHIFT", DE_KEY_RIGHT_SHIFT);
        SE::PushGlobalVariable("KEY_RIGHT_CONTROL", DE_KEY_RIGHT_CONTROL);
        SE::PushGlobalVariable("KEY_RIGHT_ALT", DE_KEY_RIGHT_ALT);
        SE::PushGlobalVariable("KEY_RIGHT_SUPER", DE_KEY_RIGHT_SUPER);
        SE::PushGlobalVariable("KEY_MENU", DE_KEY_MENU);
    }

    static void RegisterEventTypes() {
        SE::PushGlobalVariable("EVT_NONE", static_cast<int>(de::EventType::None));
        SE::PushGlobalVariable("EVT_WINDOW_CLOSE", static_cast<int>(de::EventType::WindowClose));
        SE::PushGlobalVariable("EVT_WINDOW_RESIZE", static_cast<int>(de::EventType::WindowResize));
        SE::PushGlobalVariable("EVT_WINDOW_ICONIFY", static_cast<int>(de::EventType::WindowIconify));
        SE::PushGlobalVariable("EVT_WINDOW_FOCUS", static_cast<int>(de::EventType::WindowFocus));
        SE::PushGlobalVariable("EVT_WINDOW_LOST_FOCUS", static_cast<int>(de::EventType::WindowLostFocus));
        SE::PushGlobalVariable("EVT_WINDOW_MOVED", static_cast<int>(de::EventType::WindowMoved));
        SE::PushGlobalVariable("EVT_APP_TICK", static_cast<int>(de::EventType::AppTick));
        SE::PushGlobalVariable("EVT_APP_UPDATE", static_cast<int>(de::EventType::AppUpdate));
        SE::PushGlobalVariable("EVT_APP_RENDER", static_cast<int>(de::EventType::AppRender));
        SE::PushGlobalVariable("EVT_KEY_PRESSED", static_cast<int>(de::EventType::KeyPressed));
        SE::PushGlobalVariable("EVT_KEY_RELEASE", static_cast<int>(de::EventType::KeyReleased));
        SE::PushGlobalVariable("EVT_KEY_TYPED", static_cast<int>(de::EventType::KeyTyped));
        SE::PushGlobalVariable("EVT_MOUSE_BTN_PRESSED", static_cast<int>(de::EventType::MouseBtnPressed));
        SE::PushGlobalVariable("EVT_MOUVE_BTN_RELEASED", static_cast<int>(de::EventType::MouseBtnReleased));
        SE::PushGlobalVariable("EVT_MOUSE_MOVED", static_cast<int>(de::EventType::MouseMoved));
        SE::PushGlobalVariable("EVT_MOUSE_SCROLLED", static_cast<int>(de::EventType::MouseScrolled));
    }

    static LUA_METHOD(IsKeyPressed) {
        SE::PushValue(de::Input::IsKeyPressed(SE::GetValue<int>()));
        return 1;
    }
};
}  // namespace lua
