#include "volt/gfx/global_events/GFXEventWindowMaximize.hpp"

using namespace volt::gfx;

GFXEventWindowMaximize::GFXEventWindowMaximize(Renderer *renderer,
                                               bool      maximized)
    : rend(renderer), m(maximized)
{
}
