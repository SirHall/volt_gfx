#include "volt/gfx/global_events/GFXEventWindowIconify.hpp"

using namespace volt::gfx;

GFXEventWindowIconify::GFXEventWindowIconify(Renderer *renderer, bool iconified)
    : rend(renderer), i(iconified)
{
}
