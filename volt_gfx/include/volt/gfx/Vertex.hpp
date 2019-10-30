#pragma once
#ifndef VOLT_GFX_VERTEX_HPP
#define VOLT_GFX_VERTEX_HPP

#include <GL/glew.h>
namespace volt::gfx
{
    class Vertex
    {
    private:
    protected:
    public:
        Vertex();
        Vertex(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat uv_x = 0.0f,
               GLfloat uv_y = 0.0f);
        Vertex(const Vertex &other);
        Vertex &operator=(const Vertex &other);
        // Vertex &operator=(Vertex &&other);
        ~Vertex();

        static GLuint GenerateVAO();

        GLfloat position[3] = {0.0f, 0.0f, 0.0f};
        GLfloat uv[2]       = {0.0f, 0.0f};
    };
} // namespace volt::gfx

#endif
