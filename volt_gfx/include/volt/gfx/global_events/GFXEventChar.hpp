#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCHAR_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTCHAR_HPP

#include "volt/gfx/Renderer.hpp"

namespace volt::gfx
{

    class GFXEventChar
    {
    private:
        Renderer &         renderer;
        unsigned int const character;

    public:
        GFXEventChar(Renderer &rendererRef, unsigned int charCode);

        inline Renderer &GetRenderer() { return this->renderer; }
        inline int       GetCharacter() const { return this->character; }
    };

} // namespace volt::gfx
#endif
