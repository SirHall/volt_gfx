#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCURSORPOS_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCURSORPOS_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventCursorPos
    {
    private:
        Renderer &   rend;
        double const x;
        double const y;

    public:
        GFXEventCursorPos(Renderer &renderer, double xPos, double yPos);

        inline Renderer &GetRenderer() { return this->rend; }
        inline double    GetXPos() { return this->x; }
        inline double    GetYPos() { return this->y; }
    };

} // namespace volt::gfx
#endif
