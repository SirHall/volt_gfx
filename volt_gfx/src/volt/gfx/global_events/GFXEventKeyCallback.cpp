#include "volt/gfx/global_events/GFXEventKeyCallback.hpp"

using namespace volt::gfx;

GFXEventKeyCallback::GFXEventKeyCallback(/*Renderer &rendererRef,*/ int keyCode,
                                         int scanCodeID, KeyAction keyAction,
                                         int modifiers)
    : /* renderer(rendererRef), */ key(keyCode), scanCode(scanCodeID),
      action(keyAction), mods(modifiers)
{
}
