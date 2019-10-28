#include "volt/gfx/Vertex.hpp"
#include "volt/gfx/GLUtilities.hpp"

volt::gfx::Vertex::Vertex() {}

volt::gfx::Vertex::Vertex(const volt::gfx::Vertex &other)
{
    this->position[0] = other.position[0];
    this->position[1] = other.position[1];
    this->position[2] = other.position[2];

    this->uv[0] = other.uv[0];
    this->uv[1] = other.uv[1];
}

volt::gfx::Vertex &volt::gfx::Vertex::operator=(const volt::gfx::Vertex &other)
{
    this->position[0] = other.position[0];
    this->position[1] = other.position[1];
    this->position[2] = other.position[2];

    this->uv[0] = other.uv[0];
    this->uv[1] = other.uv[1];

    return *this;
}

// volt::gfx::Vertex &volt::gfx::Vertex::operator=(volt::gfx::Vertex &&other)
// {
//     this->position[0] = other.position[0];
//     this->position[1] = other.position[1];
//     this->position[2] = other.position[2];

//     this->uv[0] = other.uv[0];
//     this->uv[1] = other.uv[1];

//     return *this;
// }

volt::gfx::Vertex::~Vertex() {}

#define GenAttrib(attr, attrType)                                              \
    GLCall(glVertexAttribPointer(                                              \
        currentIndex++, sizeof(attr) / sizeof(attrType), GL_FLOAT, GL_FALSE,   \
        sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, attr))))

GLuint volt::gfx::Vertex::GenerateVAO()
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