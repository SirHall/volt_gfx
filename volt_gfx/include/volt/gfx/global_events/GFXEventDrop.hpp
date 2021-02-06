#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTDROP_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTDROP_HPP

#include "volt/gfx/Renderer.hpp"
#include <string>
#include <vector>

namespace volt::gfx
{

    class GFXEventDrop
    {
    private:
        Renderer mutable *       rend;
        std::vector<std::string> p;

    public:
        GFXEventDrop(Renderer *renderer, int pathCount, const char *paths[]);

        inline Renderer &GetRenderer() const { return *this->rend; }
        inline std::vector<std::string> const &GetPaths() const
        {
            return this->p;
        }
    };

} // namespace volt::gfx
#endif
