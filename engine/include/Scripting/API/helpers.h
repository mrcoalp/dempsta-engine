#pragma once

#include "Input/input.h"
#include "Input/keycodes.h"
#include "Scripting/scriptengine.h"

namespace lua {
class Helpers {
public:
    static void Register() {
        RegisterKeyCodes();
        SE::RegisterFunction("IsKeyPressed", IsKeyPressed);
    }

    static void RegisterKeyCodes() {
        SE::PushGlobalVariable("DE_KEY_UNKNOWN", -1);
        SE::PushGlobalVariable("DE_KEY_SPACE", 32);
        SE::PushGlobalVariable("DE_KEY_APOSTROPHE", 39);
        SE::PushGlobalVariable("DE_KEY_COMMA", 44);
        SE::PushGlobalVariable("DE_KEY_MINUS", 45);
        SE::PushGlobalVariable("DE_KEY_PERIOD", 46);
        SE::PushGlobalVariable("DE_KEY_SLASH", 47);
        SE::PushGlobalVariable("DE_KEY_0", 48);
        SE::PushGlobalVariable("DE_KEY_1", 49);
        SE::PushGlobalVariable("DE_KEY_2", 50);
        SE::PushGlobalVariable("DE_KEY_3", 51);
        SE::PushGlobalVariable("DE_KEY_4", 52);
        SE::PushGlobalVariable("DE_KEY_5", 53);
        SE::PushGlobalVariable("DE_KEY_6", 54);
        SE::PushGlobalVariable("DE_KEY_7", 55);
        SE::PushGlobalVariable("DE_KEY_8", 56);
        SE::PushGlobalVariable("DE_KEY_9", 57);
        SE::PushGlobalVariable("DE_KEY_SEMICOLON", 59);
        SE::PushGlobalVariable("DE_KEY_EQUAL", 61);
        SE::PushGlobalVariable("DE_KEY_A", 65);
        SE::PushGlobalVariable("DE_KEY_B", 66);
        SE::PushGlobalVariable("DE_KEY_C", 67);
        SE::PushGlobalVariable("DE_KEY_D", 68);
        SE::PushGlobalVariable("DE_KEY_E", 69);
        SE::PushGlobalVariable("DE_KEY_F", 70);
        SE::PushGlobalVariable("DE_KEY_G", 71);
        SE::PushGlobalVariable("DE_KEY_H", 72);
        SE::PushGlobalVariable("DE_KEY_I", 73);
        SE::PushGlobalVariable("DE_KEY_J", 74);
        SE::PushGlobalVariable("DE_KEY_K", 75);
        SE::PushGlobalVariable("DE_KEY_L", 76);
        SE::PushGlobalVariable("DE_KEY_M", 77);
        SE::PushGlobalVariable("DE_KEY_N", 78);
        SE::PushGlobalVariable("DE_KEY_O", 79);
        SE::PushGlobalVariable("DE_KEY_P", 80);
        SE::PushGlobalVariable("DE_KEY_Q", 81);
        SE::PushGlobalVariable("DE_KEY_R", 82);
        SE::PushGlobalVariable("DE_KEY_S", 83);
        SE::PushGlobalVariable("DE_KEY_T", 84);
        SE::PushGlobalVariable("DE_KEY_U", 85);
        SE::PushGlobalVariable("DE_KEY_V", 86);
        SE::PushGlobalVariable("DE_KEY_W", 87);
        SE::PushGlobalVariable("DE_KEY_X", 88);
        SE::PushGlobalVariable("DE_KEY_Y", 89);
        SE::PushGlobalVariable("DE_KEY_Z", 90);
        SE::PushGlobalVariable("DE_KEY_LEFT_BRACKET", 91);
        SE::PushGlobalVariable("DE_KEY_BACKSLASH", 92);
        SE::PushGlobalVariable("DE_KEY_RIGHT_BRACKET", 93);
        SE::PushGlobalVariable("DE_KEY_GRAVE_ACCENT", 96);
        SE::PushGlobalVariable("DE_KEY_WORLD_1", 161);
        SE::PushGlobalVariable("DE_KEY_WORLD_2", 162);
        SE::PushGlobalVariable("DE_KEY_ESCAPE", 256);
        SE::PushGlobalVariable("DE_KEY_ENTER", 257);
        SE::PushGlobalVariable("DE_KEY_TAB", 258);
        SE::PushGlobalVariable("DE_KEY_BACKSPACE", 259);
        SE::PushGlobalVariable("DE_KEY_INSERT", 260);
        SE::PushGlobalVariable("DE_KEY_DELETE", 261);
        SE::PushGlobalVariable("DE_KEY_RIGHT", 262);
        SE::PushGlobalVariable("DE_KEY_LEFT", 263);
        SE::PushGlobalVariable("DE_KEY_DOWN", 264);
        SE::PushGlobalVariable("DE_KEY_UP", 265);
        SE::PushGlobalVariable("DE_KEY_PAGE_UP", 266);
        SE::PushGlobalVariable("DE_KEY_PAGE_DOWN", 267);
        SE::PushGlobalVariable("DE_KEY_HOME", 268);
        SE::PushGlobalVariable("DE_KEY_END", 269);
        SE::PushGlobalVariable("DE_KEY_CAPS_LOCK", 280);
        SE::PushGlobalVariable("DE_KEY_SCROLL_LOCK", 281);
        SE::PushGlobalVariable("DE_KEY_NUM_LOCK", 282);
        SE::PushGlobalVariable("DE_KEY_PRINT_SCREEN", 283);
        SE::PushGlobalVariable("DE_KEY_PAUSE", 284);
        SE::PushGlobalVariable("DE_KEY_F1", 290);
        SE::PushGlobalVariable("DE_KEY_F2", 291);
        SE::PushGlobalVariable("DE_KEY_F3", 292);
        SE::PushGlobalVariable("DE_KEY_F4", 293);
        SE::PushGlobalVariable("DE_KEY_F5", 294);
        SE::PushGlobalVariable("DE_KEY_F6", 295);
        SE::PushGlobalVariable("DE_KEY_F7", 296);
        SE::PushGlobalVariable("DE_KEY_F8", 297);
        SE::PushGlobalVariable("DE_KEY_F9", 298);
        SE::PushGlobalVariable("DE_KEY_F10", 299);
        SE::PushGlobalVariable("DE_KEY_F11", 300);
        SE::PushGlobalVariable("DE_KEY_F12", 301);
        SE::PushGlobalVariable("DE_KEY_F13", 302);
        SE::PushGlobalVariable("DE_KEY_F14", 303);
        SE::PushGlobalVariable("DE_KEY_F15", 304);
        SE::PushGlobalVariable("DE_KEY_F16", 305);
        SE::PushGlobalVariable("DE_KEY_F17", 306);
        SE::PushGlobalVariable("DE_KEY_F18", 307);
        SE::PushGlobalVariable("DE_KEY_F19", 308);
        SE::PushGlobalVariable("DE_KEY_F20", 309);
        SE::PushGlobalVariable("DE_KEY_F21", 310);
        SE::PushGlobalVariable("DE_KEY_F22", 311);
        SE::PushGlobalVariable("DE_KEY_F23", 312);
        SE::PushGlobalVariable("DE_KEY_F24", 313);
        SE::PushGlobalVariable("DE_KEY_F25", 314);
        SE::PushGlobalVariable("DE_KEY_KP_0", 320);
        SE::PushGlobalVariable("DE_KEY_KP_1", 321);
        SE::PushGlobalVariable("DE_KEY_KP_2", 322);
        SE::PushGlobalVariable("DE_KEY_KP_3", 323);
        SE::PushGlobalVariable("DE_KEY_KP_4", 324);
        SE::PushGlobalVariable("DE_KEY_KP_5", 325);
        SE::PushGlobalVariable("DE_KEY_KP_6", 326);
        SE::PushGlobalVariable("DE_KEY_KP_7", 327);
        SE::PushGlobalVariable("DE_KEY_KP_8", 328);
        SE::PushGlobalVariable("DE_KEY_KP_9", 329);
        SE::PushGlobalVariable("DE_KEY_KP_DECIMAL", 330);
        SE::PushGlobalVariable("DE_KEY_KP_DIVIDE", 331);
        SE::PushGlobalVariable("DE_KEY_KP_MULTIPLY", 332);
        SE::PushGlobalVariable("DE_KEY_KP_SUBTRACT", 333);
        SE::PushGlobalVariable("DE_KEY_KP_ADD", 334);
        SE::PushGlobalVariable("DE_KEY_KP_ENTER", 335);
        SE::PushGlobalVariable("DE_KEY_KP_EQUAL", 336);
        SE::PushGlobalVariable("DE_KEY_LEFT_SHIFT", 340);
        SE::PushGlobalVariable("DE_KEY_LEFT_CONTROL", 341);
        SE::PushGlobalVariable("DE_KEY_LEFT_ALT", 342);
        SE::PushGlobalVariable("DE_KEY_LEFT_SUPER", 343);
        SE::PushGlobalVariable("DE_KEY_RIGHT_SHIFT", 344);
        SE::PushGlobalVariable("DE_KEY_RIGHT_CONTROL", 345);
        SE::PushGlobalVariable("DE_KEY_RIGHT_ALT", 346);
        SE::PushGlobalVariable("DE_KEY_RIGHT_SUPER", 347);
        SE::PushGlobalVariable("DE_KEY_MENU", 348);
    }

    static LUA_METHOD(IsKeyPressed) {
        SE::PushValue(de::Input::IsKeyPressed(SE::GetValue<int>()));
        return 1;
    }
};
}
