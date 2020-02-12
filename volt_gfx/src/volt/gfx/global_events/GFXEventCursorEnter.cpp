#include "volt/gfx/global_events/GFXEventCursorEnter.hpp"

using namespace volt::gfx;

GFXEventCursorEnter::GFXEventCursorEnter(Renderer *renderer, bool entered)
    : rend(renderer), ent(entered)
{
}
