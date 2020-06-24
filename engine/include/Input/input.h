#pragma once

#include "Core/pch.h"
#include "Input/keycodes.h"
#include "Input/mousebuttoncodes.h"

namespace de {
class Input {
public:
    /**
     * @brief Check if given key is pressed or not.
     * @param keyCode Key to be checked.
     * @return true if given key is pressed, false otherwise.
     */
    static bool IsKeyPressed(int keyCode);

    /**
     * @brief Checks if given mouse button is pressed or not.
     * @param mouseButton Mouse button to be checked.
     * @return true if given mouse button is pressed, false otherwise.
     */
    static bool IsMouseButtonPressed(int mouseButton);

    /**
     * @brief Getter for the mouse position.
     * @return Pair of mouse position (x, y)
     */
    static std::pair<float, float> GetMousePosition();

    /**
     * @brief Getter for x coordinate mouse position.
     * @return Mouse x position.
     */
    static float GetMouseX();

    /**
     * @brief Getter for y coordinate mouse position.
     * @return Mouse y position.
     */
    static float GetMouseY();
};
}  // namespace de
