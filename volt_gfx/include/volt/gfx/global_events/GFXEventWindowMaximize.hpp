#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWMAXIMIZE_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWMAXIMIZE_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowMaximize
    {
    private:
        Renderer mutable *rend;
        bool const        m;

    public:
        GFXEventWindowMaximize(Renderer *renderer, bool maximized);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline bool      GetMaximized() const { return this->m; }
    };

} // namespace volt::gfx
#endif
