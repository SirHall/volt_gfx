#include "volt/gfx/Transform.hpp"

using namespace volt::gfx;

Transform::Transform() : transform(glm::mat4(1.0f)) {}

Transform::Transform(glm::mat4 initTransform) : transform(initTransform) {}

// Transform::Transform(const Transform &other) {}

// Transform::Transform(Transform &&other) {}

// Transform &Transform::operator=(const Transform &other) { return *this; }

// Transform &Transform::operator=(Transform &&other) { return *this; }

Transform::~Transform() {}

glm::vec3 Transform::GetPosition() const
{
    glm::vec3 transformScale;
    glm::quat transformRotation;
    glm::vec3 transformPos;
    glm::vec3 transformSkew;
    glm::vec4 transformPerspective;

    glm::decompose(transform, transformScale, transformRotation, transformPos,
                   transformSkew, transformPerspective);
    return transformPos;
}

glm::quat Transform::GetRotation() const
{
    glm::vec3 transformScale;
    glm::quat transformRotation;
    glm::vec3 transformPos;
    glm::vec3 transformSkew;
    glm::vec4 transformPerspective;

    glm::decompose(transform, transformScale, transformRotation, transformPos,
                   transformSkew, transformPerspective);
    return transformRotation;
}

void Transform::RotateLocal(glm::mat4 rotation)
{
    transform = rotation * transform;
}

void Transform::RotateGlobal(glm::mat4 rotation)
{
    glm::mat4 viewTransform = glm::translate(glm::mat4(1.0f), -GetPosition());
    viewTransform           = viewTransform * glm::toMat4(GetRotation());
    glm::mat4 inverseViewTransform = glm::inverse(viewTransform);

    transform = (viewTransform * rotation * inverseViewTransform) * transform;
}

void Transform::Translate(glm::vec3 translation)
{
    transform = glm::translate(transform, translation);
}

const glm::f32 *Transform::GetValuePtr() { return glm::value_ptr(transform); }
