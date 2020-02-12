#include "volt/gfx/global_events/GFXEventJoystick.hpp"
#include "volt/gfx/GLImport.hpp"

using namespace volt::gfx;

GFXEventJoystick::GFXEventJoystick(int jid, JoystickEvent event)
    : j(jid), e(event)
{
}

JoystickEvent::JoystickEvent(int v) : val(v) {}

JoystickEvent JoystickEvent::Connected()
{
    return JoystickEvent(GLFW_CONNECTED);
}
JoystickEvent JoystickEvent::Disconnected()
{
    return JoystickEvent(GLFW_DISCONNECTED);
}