#pragma once
#ifndef VOLT_GFX_VBO_HPP
#define VOLT_GFX_VBO_HPP

#include "volt/gfx/Buffer.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <stdexcept>
#include <vector>

namespace volt::gfx
{
    template <typename T>
    void GenVBO(GLuint vbo);

    template <typename T, GLenum VBOUsage>
    class VBO : public Buffer<T, GL_ARRAY_BUFFER, VBOUsage>
    {
    private:
    public:
        // TODO: Only allow this class to be instantiated withint volt_gfx
        VBO(GLuint vaoID) : Buffer(vaoID) { GenVBO<T>(this->vbo); }

        VBO(const VBO &other) : Buffer(other) { GenVBO<T>(this->vbo); }

        VBO &operator=(const VBO &other)
        {
            Buffer::operator=(other);
            GenVBO<T>(this->vbo);
            return *this;
        }

        VBO(VBO &&other) : Buffer(other) {}

        VBO &operator=(VBO &&other)
        {
            Buffer::operator=(other);
            return *this;
        }

        ~VBO() {}
    };
} // namespace volt::gfx
#endif
