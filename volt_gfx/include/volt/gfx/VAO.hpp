#pragma once
#ifndef VOLT_GFX_VAO_HPP
#define VOLT_GFX_VAO_HPP

#include "volt/gfx/Buffer.hpp"
#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/IBO.hpp"
#include "volt/gfx/VAOGen.hpp"
#include "volt/gfx/VBO.hpp"

namespace volt::gfx
{
    template <class VertT, class InstT>
    class VAO
    {
    private:
        GLuint                      attribIndex = 0;
        GLuint                      vao;
        VBO<VertT, GL_STATIC_DRAW>  vertVBO;
        VBO<InstT, GL_DYNAMIC_DRAW> instVBO;
        IBO                         ibo;

    public:
        VAO()
            : attribIndex(0), vao(VAOGen::GenerateVAO()),
              vertVBO(vao, attribIndex), instVBO(vao, attribIndex), ibo(vao)
        {
        }

        VAO(const VAO &other) = delete;
        // : vao(VAOGen::GenerateVAO()), vertVBO(other.vertVBO),
        //   instVBO(other.instVBO), ibo(other.ibo){}

        VAO &operator=(const VAO &other) = delete;
        // {
        //     this->attribIndex = 0;
        //     this->vao         = VAOGen::GenerateVAO();
        //     // this->vertVBO = other.vertVBO;
        //     // this->instVBO = other.instVBO;
        //     this->vertVBO = VBO<VertT, GL_STATIC_DRAW>(vao, attribIndex);
        //     return *this;
        // }

        VAO(VAO &&other)
            : vao(std::move(other.vao)), vertVBO(std::move(other.vertVBO)),
              instVBO(std::move(other.instVBO)), ibo(std::move(other.ibo))
        {
            other.vao = 0;
        }

        VAO &operator=(VAO &&other)
        {
            this->vao     = other.vao;
            this->vertVBO = std::move(other.vertVBO);
            this->instVBO = std::move(other.instVBO);
            this->ibo     = std::move(other.ibo);
            other.vao     = 0;
            return *this;
        }

        ~VAO()
        {
            GLCall(glDeleteVertexArrays(1, &this->vao));
            this->vao = 0; // Just incase
        }

        void Bind() const
        {
            GLCall(glBindVertexArray(this->vao));
            this->ibo.Bind();
        }

        VBO<VertT, GL_STATIC_DRAW> &      GetVertVBO() { return this->vertVBO; }
        VBO<VertT, GL_STATIC_DRAW> const &GetVertVBO() const
        {
            return this->vertVBO;
        }
        VBO<InstT, GL_DYNAMIC_DRAW> &GetInstVBO() { return this->instVBO; }
        VBO<InstT, GL_DYNAMIC_DRAW> const &GetInstVBO() const
        {
            return this->instVBO;
        }
        IBO &      GetIBO() { return this->ibo; }
        IBO const &GetIBO() const { return this->ibo; }

        void SetIndexBufferData(std::vector<std::size_t> const &indices)
        {
            this->Bind();
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                indices * sizeof(std::size_t), indices.data(),
                                GL_DYNAMIC_DRAW));
        }

        bool IsValid() const { return this->vao != 0; }
    };
} // namespace volt::gfx
#endif
