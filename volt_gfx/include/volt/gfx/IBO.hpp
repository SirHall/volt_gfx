#pragma once
#ifndef VOLT_GFX_IBO_HPP
#define VOLT_GFX_IBO_HPP

#include "volt/gfx/Buffer.hpp"

#include <cstdlib>

namespace volt::gfx
{
    class IBO
        : public Buffer<std::uint32_t, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW>
    {
    private:
    public:
        IBO(GLuint vaoID);
        IBO(const IBO &other);
        IBO &operator=(const IBO &other);
        IBO(IBO &&other);
        IBO &operator=(IBO &&other);
        ~IBO();
    };
} // namespace volt::gfx
#endif
