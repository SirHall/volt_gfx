#include "volt/gfx/global_events/GFXEventFramebufferSize.hpp"

using namespace volt::gfx;

GFXEventFramebufferSize::GFXEventFramebufferSize(Renderer *renderer, int width,
                                                 int height)
    : rend(renderer), w(width), h(height)
{
}
