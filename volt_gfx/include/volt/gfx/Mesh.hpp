#pragma once
#ifndef Mesh_hpp
#define Mesh_hpp

#include "volt/gfx/Vertex.hpp"

#include <cstdint>
#include <vector>

#include <GL/glew.h>
namespace volt::gfx
{
    class Mesh
    {
    private:
        GLuint vao, // Vertex Array Object
            vbo,    // Vertex Buffer Object
            ibo;    // Index Buffer Object
        std::vector<Vertex>        vertices;
        std::vector<std::uint32_t> indices;

    public:
        Mesh();
        Mesh(const Mesh &other) = delete; // Allow copying
        Mesh &operator=(const Mesh &other) = delete;
        Mesh(Mesh &&other); // Allow moving
        Mesh &operator=(Mesh &&other) = default;

        ~Mesh();

        void CreateMesh(std::vector<Vertex> &&       vertices,
                        std::vector<std::uint32_t> &&indices);
        void RenderMesh();
        void ClearMesh();

        GLuint GetVAO() const;
        GLuint GetVBO() const;
        GLuint GetIBO() const;

        const std::vector<Vertex> &       GetVertices() const;
        const std::vector<std::uint32_t> &GetIndices() const;
    };
} // namespace volt::gfx

#endif
