#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTFRAMEBUFFERSIZE_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTFRAMEBUFFERSIZE_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventFramebufferSize
    {
    private:
        Renderer mutable *rend;
        int const         w;
        int const         h;

    public:
        GFXEventFramebufferSize(Renderer *renderer, int width, int height);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline int       GetWidth() const { return this->w; }
        inline int       GetHeight() const { return this->h; }
    };

} // namespace volt::gfx
#endif
