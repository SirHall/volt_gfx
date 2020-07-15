#pragma once
#ifndef Mesh_hpp
#define Mesh_hpp

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/MeshInstanceData.hpp"
#include "volt/gfx/MeshVertex.hpp"
#include "volt/gfx/VAO.hpp"
#include "volt/gfx/VBO.hpp"

#include <cstdint>
#include <vector>

namespace volt::gfx
{
    class Mesh
    {
    private:
        VAO<MeshVertex, MeshInstanceData> vao;

        // std::vector<MeshVertex>    vertices;
        // std::vector<std::uint32_t> indices;

    public:
        Mesh();
        Mesh(const Mesh &other);
        Mesh &operator=(const Mesh &other);
        Mesh(Mesh &&other); // Allow moving
        Mesh &operator=(Mesh &&other);

        ~Mesh();

        void CreateMesh(std::vector<MeshVertex> const &   vertices,
                        std::vector<std::uint32_t> const &indices);
        void RenderMesh();

        std::vector<MeshVertex>    GetVertices() const;
        std::vector<std::uint32_t> GetIndices() const;

        bool IsValid() const;

        void Bind() const;

        VAO<MeshVertex, MeshInstanceData> &      GetVAO();
        VAO<MeshVertex, MeshInstanceData> const &GetVAO() const;
    };
} // namespace volt::gfx

#endif
