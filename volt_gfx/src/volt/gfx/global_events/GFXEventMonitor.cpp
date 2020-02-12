#include "volt/gfx/global_events/GFXEventMonitor.hpp"
#include "volt/gfx/GLImport.hpp"

using namespace volt::gfx;

GFXEventMonitor::GFXEventMonitor(GLFWmonitor *monitor, MonitorEvent event)
    : mon(monitor), e(event)
{
}

MonitorEvent::MonitorEvent(int v) : val(v) {}
MonitorEvent MonitorEvent::Connected() { return MonitorEvent(GLFW_CONNECTED); }
MonitorEvent MonitorEvent::Disconnected()
{
    return MonitorEvent(GLFW_DISCONNECTED);
}