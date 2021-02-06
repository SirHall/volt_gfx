#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWICONIFY_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWICONIFY_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowIconify
    {
    private:
        Renderer mutable *rend;
        bool const        i;

    public:
        GFXEventWindowIconify(Renderer *renderer, bool iconified);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline bool      GetIconified() const { return this->i; }
    };

} // namespace volt::gfx
#endif
