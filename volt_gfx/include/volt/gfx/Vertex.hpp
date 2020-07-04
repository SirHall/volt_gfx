#pragma once
#ifndef VOLT_GFX_VERTEX_HPP
#define VOLT_GFX_VERTEX_HPP

#include "volt/gfx/GLImport.hpp"

#include <glm/glm.hpp>

namespace volt::gfx
{
    struct Vertex
    {
    private:
    protected:
    public:
        Vertex();
        Vertex(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat uv_x = 0.0f,
               GLfloat uv_y = 0.0f);
        Vertex(const Vertex &other);
        Vertex &operator=(const Vertex &other);
        ~Vertex();

        glm::vec2 position;
        glm::vec2 uv;
    };
} // namespace volt::gfx

#endif
