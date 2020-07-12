#pragma once
#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cstdlib>
#include <vector>

template <typename T, GLenum BuffTarget, GLenum BuffUsage>
class Buffer
{
protected:
    GLuint vbo = 0;
    GLuint vao = 0; // Store parent vao

    std::size_t allocated = 0, used = 0;

public:
    Buffer(GLuint vaoID) : vao(vaoID)
    {
        GLCall(glGenBuffers(1, &this->vbo));
        this->Bind();
    }

    Buffer(const Buffer &other) : allocated(other.used), used(other.used)
    { // Setup initial buffers
        GLCall(glGenBuffers(1, &this->vbo));
        GLCall(glBindVertexArray(this->vao));
        GLCall(glBindBuffer(BuffTarget, this->vbo));
        GLCall(glBufferData(BuffTarget, this->allocated, nullptr, BuffUsage));
        // Bind for copy-pasting
        GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, this->vbo));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, other.vbo));
        GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0,
                                   0, this->allocated));
    }

    Buffer &operator=(const Buffer &other)
    {
        // Don't make this buffer larger than it has to be
        this->allocated = other.used;
        this->used      = other.used;
        // Setup initial buffers
        GLCall(glGenBuffers(1, &this->vbo));
        GLCall(glBindVertexArray(this->vao));
        GLCall(glBindBuffer(BuffTarget, this->vbo));
        GLCall(glBufferData(BuffTarget, this->allocated, nullptr, BuffUsage));
        // Bind for copy-pasting
        GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, this->vbo));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, other.vbo));
        GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0,
                                   0, this->allocated));
        return *this;
    }

    Buffer(Buffer &&other)
        : vao(other.vao), vbo(other.vbo), allocated(other.allocated),
          used(other.used)
    {
        other.vao = other.vbo = other.allocated = other.used = 0;
    }

    Buffer &operator=(Buffer &&other)
    {
        this->vao       = other.vao;
        this->vbo       = other.vbo;
        this->allocated = other.allocated;
        this->used      = other.used;
        other.vao = other.vbo = other.allocated = other.used = 0;
        return *this;
    }

    ~Buffer()
    {
        GLCall(glDeleteBuffers(1, &this->vbo));
        this->vbo = 0; // Just incase
    }

    void Bind() const
    {
        GLCall(glBindVertexArray(this->vao));
        GLCall(glBindBuffer(BuffTarget, this->vbo));
    }

    std::size_t Size() const { return this->used; }

    void Reserve(std::size_t newSize)
    {
        if (newSize == this->allocated)
            return;
        bool expand = newSize > this->allocated;
        this->Bind();
        GLuint tmpVBO = 0;
        // Temporarily copy the data to a new buffer, then copy it back
        GLCall(glGenBuffers(1, &tmpVBO));
        GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, tmpVBO));
        GLCall(glBufferData(GL_COPY_WRITE_BUFFER, this->used, nullptr,
                            GL_STATIC_READ));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, this->vbo));
        GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0,
                                   0, expand ? this->used : newSize));
        // Data has been copied to temporary storage, now expand main vbo and
        // copy back
        GLCall(glBufferData(GL_COPY_READ_BUFFER, newSize, nullptr, BuffUsage));
        GLCall(glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_COPY_READ_BUFFER, 0,
                                   0, expand ? this->used : newSize));
        GLCall(glDeleteBuffers(1, &tmpVBO));
        this->allocated = newSize;
    }

    void OptimizeReserve(std::size_t newSize)
    {
        // Expand by 1.5x, shrink when less than 0.5x
        if (newSize >= this->allocated)
            this->Reserve(newSize + (newSize / 2));
        else if (newSize < this->allocated / 2)
            this->Reserve(newSize);
    }

    void SetData(std::vector<T> const &vec)
    {
        this->Bind();
        this->OptimizeReserve(vec.size());
        GLCall(glBufferData(BuffTarget, vec.size() * sizeof(T), vec.data(),
                            BuffUsage));
    }

    T GetElement(std::size_t i)
    {
        this->Bind();
        std::size_t size = this->Size();
        if (i >= this->Size())
            throw std::runtime_error("'i' buffer index overflow for VBO");
        T val;
        GLCall(glGetBufferSubData(GL_ARRAY_BUFFER, i * sizeof(T), T,
                                  reinterpret_cast<void *>(&val)));
        return val;
    }

    void SetElement(T const &val, std::size_t i)
    {
        this->Bind();
        if (i >= this->Size())
            throw std::runtime_error("'i' buffer index overflow for VBO");
        GLCall(
            glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(T), sizeof(T), &val));
    }
    // TODO: Probably add in append/insert/remove functions
};

#endif
