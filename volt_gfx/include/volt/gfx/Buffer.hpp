#pragma once
#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/GLUtilities.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

template <typename T, GLenum BuffTarget, GLenum BuffUsage>
class Buffer
{
protected:
    GLuint vbo;
    GLuint vao; // Store parent vao

    std::size_t allocated = 0, used = 0;

public:
    Buffer(GLuint vaoID) : vao(vaoID)
    {
        GLCall(glBindVertexArray(this->vao));
        GLCall(glGenBuffers(1, &this->vbo));
    }

    Buffer(Buffer const &other) : allocated(other.used), used(other.used)
    { // Setup initial buffers

        GLCall(glGenBuffers(1, &this->vbo));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, this->vbo));
        GLCall(glBufferData(GL_COPY_READ_BUFFER, other.used * sizeof(T),
                            nullptr, BuffUsage));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, other.vbo));
        GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0,
                                   0, other.used * sizeof(T)));
    }

    Buffer &operator=(const Buffer &other)
    {
        // Don't make this buffer larger than it has to be
        this->allocated = other.used;
        this->used      = other.used;
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, this->vbo));
        GLCall(glBufferData(GL_COPY_READ_BUFFER, other.used * sizeof(T),
                            nullptr, BuffUsage));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, other.vbo));
        GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0,
                                   0, other.used * sizeof(T)));
        return *this;
    }

    Buffer(Buffer &&other)
        : vao(other.vao), vbo(other.vbo), allocated(other.allocated),
          used(other.used)
    {
        other.vao       = 0;
        other.vbo       = 0;
        other.allocated = 0;
        other.used      = 0;
    }

    Buffer &operator=(Buffer &&other)
    {
        this->vao       = other.vao;
        this->vbo       = other.vbo;
        this->allocated = other.allocated;
        this->used      = other.used;
        other.vao       = 0;
        other.vbo       = 0;
        other.allocated = 0;
        other.used      = 0;
        return *this;
    }

    ~Buffer()
    {
        if (this->vbo != 0)
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
        bool   expand = newSize > this->allocated;
        GLuint tmpVBO = 0;
        // Temporarily copy the data to a new buffer, then copy it back
        GLCall(glGenBuffers(1, &tmpVBO));
        GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, tmpVBO));
        GLCall(glBufferData(GL_COPY_WRITE_BUFFER, this->used * sizeof(T),
                            nullptr, GL_STATIC_READ));
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, this->vbo));
        GLCall(
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                                (expand ? this->used : newSize) * sizeof(T)));
        // Data has been copied to temporary storage, now expand main vbo and
        // copy back
        GLCall(glBufferData(GL_COPY_READ_BUFFER, newSize * sizeof(T), nullptr,
                            BuffUsage));
        GLCall(
            glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_COPY_READ_BUFFER, 0, 0,
                                (expand ? this->used : newSize) * sizeof(T)));
        GLCall(glDeleteBuffers(1, &tmpVBO));
        this->allocated = newSize;
    }

    void OptimizeReserve(std::size_t newSize)
    {
        // Expand by 1.5x, shrink when less than 0.5x
        if (newSize > this->allocated)
            this->Reserve(newSize + (newSize / 2));
        else if (newSize < (this->allocated / 2))
            this->Reserve(newSize);
    }

    void SetData(std::vector<T> const &vec)
    {
        this->OptimizeReserve(vec.size());
        this->used = vec.size();
        this->Bind();
        GLCall(glBufferSubData(BuffTarget, 0, vec.size() * sizeof(T),
                               reinterpret_cast<void const *>(vec.data())));
    }

    std::vector<T> GetData() const
    {
        this->Bind();
        std::vector<T> data;
        data.resize(this->Size());
        GLCall(glGetBufferSubData(BuffTarget, 0, this->Size() * sizeof(T),
                                  data.data()));
        return std::move(data);
    }

    T GetElement(std::size_t i)
    {
        this->Bind();
        std::size_t size = this->Size();
        if (i >= this->Size())
            throw std::runtime_error("'i' buffer index overflow for VBO");
        T val;
        GLCall(glGetBufferSubData(GL_ARRAY_BUFFER, i * sizeof(T), sizeof(T),
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

    void CopyTo(Buffer<T, BuffTarget, BuffUsage> &other) const
    {
        // Temporarily copy the data to a new buffer, then copy it back
        GLCall(glBindBuffer(GL_COPY_READ_BUFFER, this->vbo));
        GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, other.vbo));
        GLCall(glBufferData(GL_COPY_WRITE_BUFFER, this->Size() * sizeof(T),
                            nullptr, BuffUsage));
        GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0,
                                   0, this->Size() * sizeof(T)));
        other.used = other.allocated = this->used;
    }

    bool IsValid() const { return this->vbo != 0 && this->vao != 0; }

    // TODO: Probably add in append/insert/remove functions
};

#endif
