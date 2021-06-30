#pragma once

#include "Point3.hpp"

namespace RayMan {
  class Ray;

  class BoundingBox {
  public:
    static BoundingBox Merge(const BoundingBox& lhs, const BoundingBox& rhs);

    BoundingBox(const Point3& min, const Point3& max);

    Point3 Min() const;
    Point3 Max() const;

    bool IsHit(const Ray& ray, double distMin, double distMax) const;

  private:
    Point3 min;
    Point3 max;
  };
}  // namespace RayMan