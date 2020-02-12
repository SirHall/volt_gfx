#include "volt/gfx/global_events/GFXEventDrop.hpp"

using namespace volt::gfx;

GFXEventDrop::GFXEventDrop(Renderer *renderer, int pathCount,
                           const char *paths[])
    : rend(renderer)
{
    this->p.reserve(pathCount);
    for (int i = 0; i < pathCount; i++)
        this->p.push_back(std::string(paths[i]));
}
