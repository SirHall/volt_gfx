#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCURSORENTER_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCURSORENTER_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventCursorEnter
    {
    private:
         Renderer mutable  *rend;
        bool const        ent;

    public:
        GFXEventCursorEnter(Renderer *renderer, bool entered);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline bool      DidEnter() const { return this->ent; }
    };

} // namespace volt::gfx
#endif
