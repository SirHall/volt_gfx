#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWSIZE_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTWINDOWSIZE_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventWindowSize
    {
    private:
        Renderer mutable *renderer;
        int const         width;
        int const         height;

    public:
        GFXEventWindowSize(Renderer *rendererRef, int w, int h);

        inline Renderer &GetRenderer() const { return *this->renderer; }
        inline int       GetWidth() const { return this->width; }
        inline int       GetHeight() const { return this->height; }
    };

} // namespace volt::gfx
#endif
