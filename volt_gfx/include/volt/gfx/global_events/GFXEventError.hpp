#pragma once
#ifndef VOLT_GFX_GLOBAL_EVENTS_GFXEVENTERROR_HPP
#define VOLT_GFX_GLOBAL_EVENTS_GFXEVENTERROR_HPP

#include <string>

namespace volt::gfx
{

    struct GFXEventError
    {
    private:
        int const         code;
        std::string const msg;

    public:
        GFXEventError(int errorCode, std::string errorMsg);

        inline int                GetErrorCode() const { return this->code; }
        inline std::string const &GetErrorMsg() const { return this->msg; }
    };

} // namespace volt::gfx
#endif
