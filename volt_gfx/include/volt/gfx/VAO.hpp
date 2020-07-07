#pragma once
#ifndef VOLT_GFX_VAO_HPP
#define VOLT_GFX_VAO_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/VAOGen.hpp"
#include "volt/gfx/VBO.hpp"

namespace volt::gfx
{
    template <template <class> class VertVBO, typename VertT,
              template <class> class InstVBO, typename InstT>
    class VAO
    {
    private:
        GLuint  vao = 0;
        VertVBO vertVBO;
        InstVBO instVBO;

    public:
        VAO()
        {
            GLCall(glGenVertexArrays(1, &vao));
            this->Bind();
            this->vertVBO = VertVBO();
            this->instVBO = InstVBO();
        }

        VAO(const VAO &other)
        {
            GLCall(glGenVertexArrays(1, &vao));
            other.Bind();
        }

        VAO &operator=(const VAO &other) { return *this; }

        VAO(VAO &&other) : vao(other.vao) {}
        VAO &operator=(VAO &&other)
        {
            this->vao = other.vao;
            return *this;
        }
        ~VAO()
        {
            glDeleteVertexArrays(1, &this->vao);
            this->vao = 0; // Just incase
        }

        void Bind() const { GLCall(glBindVertexArray(this->vao)); }

        VertVBO &      GetVertVBO() { return this->vertVBO; }
        VertVBO const &GetVertVBO() { return this->vertVBO; }
        InstVBO &      GetInstVBO() { return this->instVBO; }
        InstVBO const &GetInstVBO() { return this->instVBO; }
    };
} // namespace volt::gfx
#endif
