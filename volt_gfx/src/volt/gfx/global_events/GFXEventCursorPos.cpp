#include "volt/gfx/global_events/GFXEventCursorPos.hpp"

using namespace volt::gfx;

GFXEventCursorPos::GFXEventCursorPos(Renderer *renderer, double xPos,
                                     double yPos)
    : rend(renderer), x(xPos), y(yPos)
{
}
