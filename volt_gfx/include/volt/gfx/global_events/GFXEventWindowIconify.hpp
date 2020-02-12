#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWICONIFY_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWICONIFY_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowIconify
    {
    private:
        Renderer * rend;
        bool const i;

    public:
        GFXEventWindowIconify(Renderer *renderer, bool iconified);

        inline Renderer &GetRenderer() { return *this->rend; }
        inline bool      GetIconified() { return this->i; }
    };

} // namespace volt::gfx
#endif
