#pragma once
#ifndef Mesh_hpp
#define Mesh_hpp

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/Vertex.hpp"

#include <cstdint>
#include <vector>

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
        Mesh(const Mesh &other); // Allow copying
        Mesh &operator=(const Mesh &other);
        Mesh(Mesh &&other); // Allow moving
        Mesh &operator=(Mesh &&other);

        ~Mesh();

        void CreateMesh(std::vector<Vertex> const &       vertices,
                        std::vector<std::uint32_t> const &indices);
        void RenderMesh();
        void ClearMesh();

        GLuint GetVAO() const;
        GLuint GetVBO() const;
        GLuint GetIBO() const;

        const std::vector<Vertex> &       GetVertices() const;
        const std::vector<std::uint32_t> &GetIndices() const;

        bool IsValid() const;

        void Bind() const;
        void Unbind() const;
    };
} // namespace volt::gfx

#endif
