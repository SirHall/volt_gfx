#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWREFRESH_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWREFRESH_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowRefresh
    {
    private:
        Renderer *rend;

    public:
        GFXEventWindowRefresh(Renderer *renderer);

        inline Renderer &GetRenderer() { return *this->rend; }
    };

} // namespace volt::gfx
#endif
