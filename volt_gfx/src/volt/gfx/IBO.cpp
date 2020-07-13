#include "volt/gfx/IBO.hpp"

using namespace volt::gfx;

IBO::IBO(GLuint vaoID) : Buffer(vaoID) {}

// IBO::IBO(const IBO &other) : Buffer(other) {}

// IBO &IBO::operator=(const IBO &other)
// {
//     Buffer::operator=(other);
//     return *this;
// }

IBO::IBO(IBO &&other) : Buffer(std::move(other)) {}

IBO &IBO::operator=(IBO &&other)
{
    Buffer::operator=(std::move(other));
    return *this;
}

IBO::~IBO() {}
