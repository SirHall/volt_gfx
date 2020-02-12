#include "volt/gfx/global_events/GFXEventWindowContentScale.hpp"

using namespace volt::gfx;

GFXEventWindowContentScale::GFXEventWindowContentScale(Renderer *renderer,
                                                       float     xScale,
                                                       float     yScale)
    : rend(renderer), x(xScale), y(yScale)
{
}
