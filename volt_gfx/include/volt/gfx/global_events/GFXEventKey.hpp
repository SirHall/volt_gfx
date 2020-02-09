#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTKEY_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTKEY_HPP

#include "volt/gfx/Renderer.hpp"

// OpenGL Start
#include <GL/glew.h>
// glew must be imported before glfw3
#include <GLFW/glfw3.h>
// OpenGL End

using namespace volt::gfx;

namespace volt::gfx
{

    class KeyAction
    {
    private:
        int const val;

    public:
        KeyAction(int v);
        inline int Get() const { return val; }

        static KeyAction Press();
        static KeyAction Release();
        static KeyAction Hold();
        static KeyAction Unknown();
    };

    struct GFXEventKey
    {
    private:
        Renderer &      renderer;
        int const       key;
        int const       scanCode;
        KeyAction const action;
        int             mods;

    public:
        GFXEventKey(Renderer &rendererRef, int keyCode, int scanCodeID,
                    KeyAction keyAction, int modifiers);

        inline Renderer &GetRenderer() { return this->renderer; }
        inline int       GetKey() const { return this->key; }
        inline int       GetScanCode() const { return this->scanCode; }
        inline KeyAction GetAction() const { return this->action; }
    };

} // namespace volt::gfx

#endif
