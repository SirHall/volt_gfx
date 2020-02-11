#include "volt/gfx/global_events/GFXEventChar.hpp"

using namespace volt::gfx;

GFXEventChar::GFXEventChar(Renderer *rendererRef, unsigned int charCode)
    : renderer(rendererRef), character(charCode)
{
}
