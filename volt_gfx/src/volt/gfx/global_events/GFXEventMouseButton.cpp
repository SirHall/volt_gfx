#include "volt/gfx/global_events/GFXEventMouseButton.hpp"

using namespace volt::gfx;

GFXEventMouseButton::GFXEventMouseButton(Renderer &        renderer,
                                         MouseButton       mouseButton,
                                         MouseButtonAction buttonAction,
                                         int               modifiers)
    : rend(renderer), button(mouseButton), action(buttonAction), mods(modifiers)
{
}
