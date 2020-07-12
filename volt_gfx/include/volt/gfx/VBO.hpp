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
    GLuint GenVBO(GLuint vbo, GLuint attribIndex);

    template <typename T, GLenum VBOUsage>
    class VBO : public Buffer<T, GL_ARRAY_BUFFER, VBOUsage>
    {
    private:
        GLuint initAttribIndex;

    public:
        // TODO: Only allow this class to be instantiated withint volt_gfx
        VBO(GLuint vaoID, GLuint &attribIndex)
            : Buffer(vaoID), initAttribIndex(attribIndex)
        {
            this->Bind();
            attribIndex = GenVBO<T>(this->vbo, attribIndex);
        }

        VBO(const VBO &other)
            : Buffer(other), initAttribIndex(other.initAttribIndex)
        {
            this->Bind();
            GenVBO<T>(this->vbo, this->initAttribIndex);
        }

        VBO &operator=(const VBO &other)
        {
            Buffer::operator      =(other);
            this->initAttribIndex = other.initAttribIndex;
            this->Bind();
            GenVBO<T>(this->vbo, this->initAttribIndex);
            return *this;
        }

        VBO(VBO &&other) : Buffer(other), initAttribIndex(other.initAttribIndex)
        {
        }

        VBO &operator=(VBO &&other)
        {
            Buffer::operator      =(other);
            this->initAttribIndex = other.initAttribIndex;
            return *this;
        }

        ~VBO() {}
    };
} // namespace volt::gfx
#endif
