#include "volt/gfx/global_events/GFXEventWindowFocus.hpp"

using namespace volt::gfx;

GFXEventWindowFocus::GFXEventWindowFocus(Renderer *renderer, bool focused)
    : rend(renderer), f(focused)
{
}