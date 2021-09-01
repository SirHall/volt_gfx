#include "volt/gfx/Camera.hpp"

#include <memory>

using namespace volt::gfx;

Camera::Camera() {}

Camera::Camera(const Camera &other)
    : transform(other.transform), projectionDirty(other.projectionDirty),
      projection(other.projection), isPerspective(other.isPerspective),
      ratio(other.ratio), orthoSize(other.orthoSize),
      perspectiveFov(other.perspectiveFov), nearPlane(other.nearPlane),
      farPlane(other.farPlane)
{
}

Camera &Camera::operator=(const Camera &other)
{
    transform       = other.transform;
    projectionDirty = other.projectionDirty;
    projection      = other.projection;
    isPerspective   = other.isPerspective;
    ratio           = other.ratio;
    orthoSize       = other.orthoSize;
    perspectiveFov  = other.perspectiveFov;
    nearPlane       = other.nearPlane;
    farPlane        = other.farPlane;
    return *this;
}

Camera::Camera(Camera &&other)
    : transform(std::move(other.transform)),
      projectionDirty(std::move(other.projectionDirty)),
      projection(std::move(other.projection)),
      isPerspective(std::move(other.isPerspective)),
      ratio(std::move(other.ratio)), orthoSize(std::move(other.orthoSize)),
      perspectiveFov(std::move(other.perspectiveFov)),
      nearPlane(std::move(other.nearPlane)), farPlane(std::move(other.farPlane))
{
}

Camera &Camera::operator=(Camera &&other)
{
    transform       = std::move(other.transform);
    projectionDirty = std::move(other.projectionDirty);
    projection      = std::move(other.projection);
    isPerspective   = std::move(other.isPerspective);
    ratio           = std::move(other.ratio);
    orthoSize       = std::move(other.orthoSize);
    perspectiveFov  = std::move(other.perspectiveFov);
    nearPlane       = std::move(other.nearPlane);
    farPlane        = std::move(other.farPlane);
    return *this;
}

Camera::~Camera() {}

Transform const &Camera::GetTransform() const { return this->transform; }
Transform &      Camera::GetTransform() { return this->transform; }
void             Camera::SetTransform(Transform v) { this->transform = v; }

glm::mat4 const &Camera::GetProjection() const
{
    if (this->projectionDirty)
    {
        this->projectionDirty = false;
        this->projection =
            this->isPerspective
                ? glm::perspective(this->perspectiveFov, this->ratio,
                                   this->nearPlane, this->farPlane)
                : glm::ortho((this->ratio * this->orthoSize) * -0.5f,
                             (this->ratio * this->orthoSize) * 0.5f,
                             this->orthoSize * 0.5f, this->orthoSize * -0.5f,
                             this->nearPlane, this->farPlane);
    }
    return this->projection;
}

glm::mat4 Camera::GetPV() const
{
    return this->GetProjection() * this->transform.GetMatrix();
}

bool Camera::IsPerspective() const { return this->isPerspective; }
void Camera::SetPerspectiveMode(bool mode)
{
    this->isPerspective   = mode;
    this->projectionDirty = true;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->ratio           = aspectRatio;
    this->projectionDirty = true;
}

void Camera::SetOrthoSize(float size)
{
    this->orthoSize       = size;
    this->projectionDirty = true;
}
void Camera::SetPerspectiveFOV(float fov)
{
    this->perspectiveFov  = fov;
    this->projectionDirty = true;
}

void Camera::SetNearFarPlanes(float near, float far)
{
    this->nearPlane       = near;
    this->farPlane        = far;
    this->projectionDirty = true;
}