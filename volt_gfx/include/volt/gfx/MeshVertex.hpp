#pragma once
#ifndef VOLT_GFX_MESHVERTEX_HPP
#define VOLT_GFX_MESHVERTEX_HPP

#include "volt/gfx/GLImport.hpp"
#include "volt/gfx/VBO.hpp"

#include <glm/glm.hpp>

namespace volt::gfx
{
    struct MeshVertex
    {
    private:
    protected:
    public:
        MeshVertex();
        MeshVertex(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
                   GLfloat uv_x = 0.0f, GLfloat uv_y = 0.0f);
        MeshVertex(const MeshVertex &other);
        MeshVertex &operator=(const MeshVertex &other);
        ~MeshVertex();

        glm::vec3 position;
        glm::vec2 uv;
    };

    template <>
    GLuint GenVBO<MeshVertex>(GLuint vbo, GLuint attribIndex);
} // namespace volt::gfx

#endif
