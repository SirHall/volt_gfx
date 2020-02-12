#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWFOCUS_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWFOCUS_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowFocus
    {
    private:
        Renderer *rend;
        bool      f;

    public:
        GFXEventWindowFocus(Renderer *renderer, bool focused);

        inline Renderer &GetRenderer() { return *this->rend; }
        inline bool      GetFocused() { return this->f; }
    };

} // namespace volt::gfx
#endif
