#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTSCROLL_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTSCROLL_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventScroll
    {
    private:
        Renderer mutable *rend;
        double const      x;
        double const      y;

    public:
        GFXEventScroll(Renderer *renderer, double xOffset, double yOffset);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline double    GetXOffset() const { return this->x; }
        inline double    GetYOffset() const { return this->y; }
    };

} // namespace volt::gfx
#endif
