#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCURSORPOS_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCURSORPOS_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventCursorPos
    {
    private:
        Renderer mutable *rend;
        double const      x;
        double const      y;

    public:
        GFXEventCursorPos(Renderer *renderer, double xPos, double yPos);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline double    GetXPos() const { return this->x; }
        inline double    GetYPos() const { return this->y; }
    };

} // namespace volt::gfx
#endif
