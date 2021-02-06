#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTKEY_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTKEY_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Renderer.hpp"

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
        Renderer mutable *renderer;
        int const         key;
        int const         scanCode;
        KeyAction const   action;
        int               mods;

    public:
        GFXEventKey(Renderer *rendererRef, int keyCode, int scanCodeID,
                    KeyAction keyAction, int modifiers);

        inline Renderer &GetRenderer() const { return *this->renderer; }
        inline int       GetKey() const { return this->key; }
        inline int       GetScanCode() const { return this->scanCode; }
        inline KeyAction GetAction() const { return this->action; }
    };

} // namespace volt::gfx

#endif
