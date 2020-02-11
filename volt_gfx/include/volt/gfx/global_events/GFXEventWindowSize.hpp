#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWSIZE_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWSIZE_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowSize
    {
    private:
        Renderer *renderer;
        int const width;
        int const height;

    public:
        GFXEventWindowSize(Renderer *rendererRef, int w, int h);

        inline Renderer &GetRenderer() { return *this->renderer; }
        inline int       GetWidth() { return this->width; }
        inline int       GetHeight() { return this->height; }
    };

} // namespace volt::gfx
#endif
