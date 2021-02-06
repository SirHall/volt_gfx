#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTMOUSEBUTTON_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTMOUSEBUTTON_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Renderer.hpp"

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
        Renderer mutable* rend;
        MouseButton const       button;
        MouseButtonAction const action;
        int const               mods;

    public:
        GFXEventMouseButton(Renderer *renderer, MouseButton mouseButton,
                            MouseButtonAction buttonAction, int modifiers);

        inline Renderer &        GetRenderer() const { return *this->rend; }
        inline MouseButton       GetButton() const { return this->button; }
        inline MouseButtonAction GetAction() const { return this->action; }
    };

} // namespace volt::gfx
#endif
