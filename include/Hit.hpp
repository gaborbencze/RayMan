#pragma once

#include <Point3.hpp>
#include <UnitVector3.hpp>

namespace RayMan {
  class Ray;
  class Material;

  struct Hit {
    Point3 point;
    UnitVector3 normal;
    const Material* material;

    Hit(const Point3& point, const UnitVector3& outwardNormal, const Ray& ray,
        const Material* material);
  };
}  // namespace RayMan