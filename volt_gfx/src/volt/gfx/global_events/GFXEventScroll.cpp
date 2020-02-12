#include "volt/gfx/global_events/GFXEventScroll.hpp"

using namespace volt::gfx;

GFXEventScroll::GFXEventScroll(Renderer *renderer, double xOffset,
                               double yOffset)
    : rend(renderer), x(xOffset), y(yOffset)
{
}
