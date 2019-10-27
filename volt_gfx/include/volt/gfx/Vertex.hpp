#pragma once
#ifndef VOLT_GFX_VERTEX_HPP
#define VOLT_GFX_VERTEX_HPP

#include <GL/glew.h>

class Vertex
{
private:
protected:
public:
    Vertex();
    Vertex(const Vertex &other);
    Vertex &operator=(const Vertex &other);
    Vertex &operator=(Vertex &&other);
    ~Vertex();

    static GLuint GenerateVAO();

    GLfloat position[3] = {0.0f, 0.0f, 0.0f};
    GLfloat uv[2]       = {0.0f, 0.0f};
};

#endif
