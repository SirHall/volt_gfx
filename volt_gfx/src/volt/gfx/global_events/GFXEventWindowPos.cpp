#include "volt/gfx/global_events/GFXEventWindowPos.hpp"

using namespace volt::gfx;

GFXEventWindowPos::GFXEventWindowPos(Renderer *renderer, int xPos, int yPos)
    : rend(renderer), x(xPos), y(yPos)
{
}
