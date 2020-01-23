#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTMOUSEBUTTON_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTMOUSEBUTTON_HPP

#include "volt/gfx/Renderer.hpp"

// OpenGL Start
#include <GL/glew.h>
// glew must be imported before glfw3
#include <GLFW/glfw3.h>
// OpenGL End

namespace volt::gfx
{
    enum MouseButtonAction
    {
        Press   = GLFW_PRESS,
        Release = GLFW_RELEASE
    };

    enum MouseButton
    {
        Left   = GLFW_MOUSE_BUTTON_LEFT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
        Right  = GLFW_MOUSE_BUTTON_RIGHT,
        _1     = GLFW_MOUSE_BUTTON_1,
        _2     = GLFW_MOUSE_BUTTON_2,
        _3     = GLFW_MOUSE_BUTTON_3,
        _4     = GLFW_MOUSE_BUTTON_4,
        _5     = GLFW_MOUSE_BUTTON_5,
        _6     = GLFW_MOUSE_BUTTON_6,
        _7     = GLFW_MOUSE_BUTTON_7,
        _8     = GLFW_MOUSE_BUTTON_8,
        Last   = GLFW_MOUSE_BUTTON_LAST
    };

    class GFXEventMouseButton
    {
    private:
        Renderer &              rend;
        MouseButton const       button;
        MouseButtonAction const action;
        int const               mods;

    public:
        GFXEventMouseButton(Renderer &renderer, MouseButton mouseButton,
                            MouseButtonAction buttonAction, int modifiers);

        inline Renderer &        GetRenderer() { return this->rend; }
        inline MouseButton       GetButton() { return this->button; }
        inline MouseButtonAction GetAction() { return this->action; }
    };

} // namespace volt::gfx
#endif
