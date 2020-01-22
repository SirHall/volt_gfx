#include "volt/gfx/global_events/GFXEventError.hpp"

using namespace volt::gfx;

GFXEventError::GFXEventError(int errorCode, std::string errorMsg)
    : code(errorCode), msg(errorMsg)
{
}
