#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWCONTENTSCALE_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWCONTENTSCALE_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowContentScale
    {
    private:
        Renderer *rend;
        float     x;
        float     y;

    public:
        GFXEventWindowContentScale(Renderer *renderer, float xScale,
                                   float yScale);

        inline Renderer &GetRenderer() { return *this->rend; }
        inline float     GetXScale() { return this->x; }
        inline float     GetYScale() { return this->y; }
    };

} // namespace volt::gfx
#endif
