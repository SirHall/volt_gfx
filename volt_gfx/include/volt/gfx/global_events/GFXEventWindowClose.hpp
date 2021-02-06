#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWCLOSE_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWCLOSE_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowClose
    {
    private:
        Renderer mutable *rend;

    public:
        GFXEventWindowClose(Renderer *renderer);
        inline Renderer &GetRenderer() const { return *this->rend; }
    };

} // namespace volt::gfx
#endif
