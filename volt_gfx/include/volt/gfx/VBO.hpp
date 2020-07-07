#pragma once
#ifndef VOLT_GFX_VBO_HPP
#define VOLT_GFX_VBO_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <stdexcept>
#include <vector>

namespace volt::gfx
{
    template <typename T>
    void GenVBO(GLuint vbo);

    template <typename T>
    class VBO
    {
    private:
        GLuint vbo = 0;
        GLuint vao = 0; // Store parent vao

    public:
        // TODO: Only allow this class to be instantiated withint volt_gfx
        VBO()
        {
            GLCall(glGenBuffers(1, &this->vbo));
            this->Bind();
            GenVBO<T>(this->vbo);
        }

        VBO(const VBO &other)
        {
            glGenBuffers(1, &this->vbo);
            glCopyBufferSubData(other.vbo, this->vbo, 0, 0, other.ByteCount());
            GenVBO<T>(this->vbo);
        }

        VBO &operator=(const VBO &other)
        {
            glGenBuffers(1, &this->vbo);
            glCopyBufferSubData(other.vbo, this->vbo, 0, 0, other.ByteCount());
            GenVBO<T>(this->vbo);
            return *this;
        }

        VBO(VBO &&other)
        {
            this->vbo = other.vbo;
            other.vbo = 0;
        }

        VBO &operator=(VBO &&other)
        {
            this->vbo = other.vbo;
            other.vbo = 0;
            return *this;
        }

        ~VBO()
        {
            if (this->vbo != 0)
                GLCall(glDeleteBuffers(1, &this->vbo));
        }

        void Bind() const
        {
            GLCall(glBindVertexArray(this->vao));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->vbo));
        }

        GLuint ByteCount() const
        {
            this->Bind();
            GLint size = 0;
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE,
                                   &size); // In bytes
            return GLuint(size);
        }

        std::size_t Size() const { return this->ByteCount() / sizeof(T); }

        T operator[](std::size_t i)
        {
            this->Bind();
            std::size_t size = this->Size();
            if (i >= this->Size())
                throw std::runtime_error("'i' index buffer overflow for VBO");
            T val;
            glGetBufferSubData(GL_ARRAY_BUFFER, i * sizeof(T), T,
                               reinterpret_cast<void *>(&val));
            return val;
        }
    };
} // namespace volt::gfx
#endif
