#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWPOS_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWPOS_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowPos
    {
    private:
        Renderer *rend;
        int const x;
        int const y;

    public:
        GFXEventWindowPos(Renderer *renderer, int xPos, int yPos);

        inline Renderer &GetRenderer() { return *this->rend; }
        inline int       GetXPos() { return this->x; }
        inline int       GetYPos() { return this->y; }
    };

} // namespace volt::gfx
#endif
