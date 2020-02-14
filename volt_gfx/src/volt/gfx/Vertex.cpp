#include "volt/gfx/Vertex.hpp"
#include "volt/gfx/GLUtilities.hpp"

volt::gfx::Vertex::Vertex() {}

volt::gfx::Vertex::Vertex(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
                          GLfloat uv_x, GLfloat uv_y)
{
    position[0] = pos_x;
    position[1] = pos_y;
    position[2] = pos_z;

    uv[0] = uv_x;
    uv[1] = uv_y;
}

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

volt::gfx::Vertex::~Vertex() {}

// #define GenAttrib(attr, attrType)                                              \
//     GLCall(gl::VertexAttribPointer(                                            \
//         currentIndex++, sizeof(attr) / sizeof(attrType),                       \
//         GetGlType<attrType>(), gl::FALSE_, sizeof(Vertex),                     \
//         reinterpret_cast<void *>(offsetof(Vertex, attr))))

GLuint volt::gfx::Vertex::GenerateVAO()
{
    GLuint vao = 0;
    GLCall(gl::GenVertexArrays(2, &vao));
    GLCall(gl::BindVertexArray(vao));
    // Vertex attributes
    GLCall(gl::EnableVertexAttribArray(0));
    GLCall(gl::EnableVertexAttribArray(1));

    GLuint currentIndex = 0;

    // Model position attribute
    // GenAttrib(position, GLfloat);
    GLCall(gl::VertexAttribPointer(
        currentIndex, 3, gl::FLOAT, gl::FALSE_, sizeof(Vertex),
        reinterpret_cast<void *>(offsetof(Vertex, position))));

    currentIndex++;

    // UV Coordinate Attribute
    // GenAttrib(uv, GLfloat);
    GLCall(gl::VertexAttribPointer(
        currentIndex, 2, gl::FLOAT, gl::FALSE_, sizeof(Vertex),
        reinterpret_cast<void *>(offsetof(Vertex, uv))));

    currentIndex++;

    return vao;
}