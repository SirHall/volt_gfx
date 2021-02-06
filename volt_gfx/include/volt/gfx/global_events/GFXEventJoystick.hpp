#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTJOYSTICK_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTJOYSTICK_HPP

namespace volt::gfx
{
    struct JoystickEvent
    {
    private:
        int const val;

    public:
        JoystickEvent(int v);
        inline int Get() const { return val; }

        static JoystickEvent Connected();
        static JoystickEvent Disconnected();
    };

    class GFXEventJoystick
    {
    private:
        int const           j;
        JoystickEvent const e;

    public:
        GFXEventJoystick(int jid, JoystickEvent event);

        inline int           GetJoystickID() const { return this->j; }
        inline JoystickEvent GetJoystickEvent() const { return this->e; }
    };

} // namespace volt::gfx
#endif
