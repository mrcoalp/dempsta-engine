#include "Input/input.h"

#include "Core/application.h"

namespace de {
    bool Input::IsKeyPressed(int keyCode) {
        auto window = Application::GetInstance().GetWindow().GetNativeWindow();
        auto state = glfwGetKey(window, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int mouseButton) {
        auto window = Application::GetInstance().GetWindow().GetNativeWindow();
        auto state = glfwGetMouseButton(window, mouseButton);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition() {
        auto window = Application::GetInstance().GetWindow().GetNativeWindow();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    float Input::GetMouseX() {
        auto[x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY() {
        auto[x, y] = GetMousePosition();
        return y;
    }
}
