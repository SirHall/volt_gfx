#include "volt/gfx/global_events/GFXEventKey.hpp"

using namespace volt::gfx;

GFXEventKey::GFXEventKey(Renderer *rendererRef, int keyCode, int scanCodeID,
                         KeyAction keyAction, int modifiers)
    : renderer(rendererRef), key(keyCode), scanCode(scanCodeID),
      action(keyAction), mods(modifiers)
{
}

KeyAction::KeyAction(int v) : val(v) {}
KeyAction KeyAction::Press() { return KeyAction(GLFW_PRESS); }
KeyAction KeyAction::Release() { return KeyAction(GLFW_RELEASE); }
KeyAction KeyAction::Hold() { return KeyAction(GLFW_REPEAT); }
KeyAction KeyAction::Unknown() { return KeyAction(GLFW_KEY_UNKNOWN); }