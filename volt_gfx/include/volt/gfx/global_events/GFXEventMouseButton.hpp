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
    struct MouseButtonAction
    {
    private:
        int const val;

    public:
        MouseButtonAction(int v);
        inline int Get() const { return val; }

        static MouseButtonAction Press();
        static MouseButtonAction Release();
    };

    struct MouseButton
    {
    private:
        int const val;

    public:
        MouseButton(int v);
        inline int Get() const { return val; }

        static MouseButton Left();
        static MouseButton Middle();
        static MouseButton Right();
        static MouseButton Num1();
        static MouseButton Num2();
        static MouseButton Num3();
        static MouseButton Num4();
        static MouseButton Num5();
        static MouseButton Num6();
        static MouseButton Num7();
        static MouseButton Num8();
        static MouseButton Last();
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
