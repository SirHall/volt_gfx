#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTMONITOR_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTMONITOR_HPP

#include "volt/gfx/GLImport.hpp"

namespace volt::gfx
{

    struct MonitorEvent
    {
    private:
        int const val;

    public:
        MonitorEvent(int v);

        static MonitorEvent Connected();
        static MonitorEvent Disconnected();
    };

    class GFXEventMonitor
    {
    private:
        GLFWmonitor *      mon;
        MonitorEvent const e;

    public:
        GFXEventMonitor(GLFWmonitor *monitor, MonitorEvent event);

        GLFWmonitor &GetMonitor() { return *this->mon; }
        MonitorEvent GetMonitorEvent() { return this->e; }
    };

} // namespace volt::gfx
#endif
