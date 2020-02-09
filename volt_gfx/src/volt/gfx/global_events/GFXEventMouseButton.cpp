#include "volt/gfx/global_events/GFXEventMouseButton.hpp"

using namespace volt::gfx;

GFXEventMouseButton::GFXEventMouseButton(Renderer &        renderer,
                                         MouseButton       mouseButton,
                                         MouseButtonAction buttonAction,
                                         int               modifiers)
    : rend(renderer), button(mouseButton), action(buttonAction), mods(modifiers)
{
}

MouseButtonAction::MouseButtonAction(int v) : val(v) {}
MouseButtonAction MouseButtonAction::Press()
{
    return MouseButtonAction(GLFW_PRESS);
}
MouseButtonAction MouseButtonAction::Release()
{
    return MouseButtonAction(GLFW_RELEASE);
}

MouseButton::MouseButton(int v) : val(v) {}
MouseButton MouseButton::Left() { return MouseButton(GLFW_MOUSE_BUTTON_LEFT); }
MouseButton MouseButton::Middle()
{
    return MouseButton(GLFW_MOUSE_BUTTON_MIDDLE);
}
MouseButton MouseButton::Right()
{
    return MouseButton(GLFW_MOUSE_BUTTON_RIGHT);
}
MouseButton MouseButton::Num1() { return MouseButton(GLFW_MOUSE_BUTTON_1); }
MouseButton MouseButton::Num2() { return MouseButton(GLFW_MOUSE_BUTTON_2); }
MouseButton MouseButton::Num3() { return MouseButton(GLFW_MOUSE_BUTTON_3); }
MouseButton MouseButton::Num4() { return MouseButton(GLFW_MOUSE_BUTTON_4); }
MouseButton MouseButton::Num5() { return MouseButton(GLFW_MOUSE_BUTTON_5); }
MouseButton MouseButton::Num6() { return MouseButton(GLFW_MOUSE_BUTTON_6); }
MouseButton MouseButton::Num7() { return MouseButton(GLFW_MOUSE_BUTTON_7); }
MouseButton MouseButton::Num8() { return MouseButton(GLFW_MOUSE_BUTTON_8); }
MouseButton MouseButton::Last() { return MouseButton(GLFW_MOUSE_BUTTON_LAST); }