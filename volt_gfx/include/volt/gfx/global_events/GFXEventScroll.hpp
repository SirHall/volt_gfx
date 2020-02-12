#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTSCROLL_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTSCROLL_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventScroll
    {
    private:
        Renderer *   rend;
        double const x;
        double const y;

    public:
        GFXEventScroll(Renderer *renderer, double xOffset, double yOffset);

        inline Renderer &GetRenderer() { return *this->rend; }
        inline double    GetXOffset() { return this->x; }
        inline double    GetYOffset() { return this->y; }
    };

} // namespace volt::gfx
#endif
