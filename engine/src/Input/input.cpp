#include "Input/input.h"

#include <GLFW/glfw3.h>

#include "Core/application.h"

namespace de {
bool Input::IsKeyPressed(int keyCode) {
    auto* _window = Application::GetInstance().GetWindow().GetNativeWindow();
    auto _state = glfwGetKey(_window, keyCode);
    return _state == GLFW_PRESS || _state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int mouseButton) {
    auto* _window = Application::GetInstance().GetWindow().GetNativeWindow();
    auto _state = glfwGetMouseButton(_window, mouseButton);
    return _state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() {
    auto* _window = Application::GetInstance().GetWindow().GetNativeWindow();
    double _x;
    double _y;

    glfwGetCursorPos(_window, &_x, &_y);
    return {static_cast<float>(_x), static_cast<float>(_y)};
}

float Input::GetMouseX() {
    auto [_x, _y] = GetMousePosition();
    return _x;
}

float Input::GetMouseY() {
    auto [_x, _y] = GetMousePosition();
    return _y;
}
}  // namespace de
