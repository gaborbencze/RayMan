#pragma once

#include <Point3.hpp>
#include <Vector3.hpp>

namespace RayMan {

  class Ray;

  class Camera {
  public:
    Camera();

    Ray GetRay(double u, double v) const;

  private:
    Point3 origin;
    Vector3 horizontal;
    Vector3 vertical;
    Point3 lowerLeftCorner;
  };
}  // namespace RayMan