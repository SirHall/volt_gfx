#include "volt/gfx/global_events/GFXEventKey.hpp"

using namespace volt::gfx;

GFXEventKey::GFXEventKey(Renderer &rendererRef, int keyCode, int scanCodeID,
                         KeyAction keyAction, int modifiers)
    : renderer(rendererRef), key(keyCode), scanCode(scanCodeID),
      action(keyAction), mods(modifiers)
{
}
