#pragma once

#include "Core/pch.h"

namespace de {
    class Input {
    public:
        static bool IsKeyPressed(int keyCode);

        static bool IsMouseButtonPressed(int mouseButton);

        static std::pair<float, float> GetMousePosition();

        static float GetMouseX();

        static float GetMouseY();
    };
}
