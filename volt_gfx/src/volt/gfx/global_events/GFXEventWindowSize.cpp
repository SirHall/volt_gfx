#include "volt/gfx/global_events/GFXEventWindowSize.hpp"

using namespace volt::gfx;

GFXEventWindowSize::GFXEventWindowSize(Renderer *rendererRef, int w, int h)
    : renderer(rendererRef), width(w), height(h)
{
}
