#pragma once

#include <Point3.hpp>
#include <UnitVector3.hpp>

namespace RayMan {
  class Ray;

  struct Hit {
    Point3 point;
    UnitVector3 normal;

    Hit(const Point3& point, const UnitVector3& outwardNormal, const Ray& ray);
  };
}  // namespace RayMan