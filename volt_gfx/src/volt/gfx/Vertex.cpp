#include "volt/gfx/Vertex.hpp"
#include "volt/gfx/GLUtilities.hpp"

Vertex::Vertex() {}

Vertex::Vertex(const Vertex &other) {}

Vertex &Vertex::operator=(const Vertex &other) { return *this; }

Vertex &Vertex::operator=(Vertex &&other) { return *this; }

Vertex::~Vertex() {}

#define GenAttrib(attr, attrType)                                              \
    GLCall(glVertexAttribPointer(                                              \
        currentIndex++, sizeof(attr) / sizeof(attrType), GL_FLOAT, GL_FALSE,   \
        sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, attr))))

GLuint Vertex::GenerateVAO()
{
    GLuint vao = 0;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    // Vertex attributes
    GLCall(glEnableVertexAttribArray(0));

    GLuint currentIndex = 0;

    // Model position attribute
    GenAttrib(position, GLfloat);
    // GLCall(glVertexAttribPointer(
    //     currentIndex++, sizeof(position) / sizeof(GLfloat), GL_FLOAT,
    //     GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex,
    //     position))));

    // UV Coordinate Attribute
    GenAttrib(uv, GLfloat);
    // GLCall(glVertexAttribPointer(
    //     currentIndex++, sizeof(uv) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
    //     sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, uv))));

    return vao;
}