#pragma once

#include <Point3.hpp>
#include <UnitVector3.hpp>

namespace RayMan {
  class Ray {
  public:
    Ray(const Point3& origin, const UnitVector3& direction);

    Point3 GetOrigin() const;
    UnitVector3 GetDirection() const;

    Point3 At(double distance) const;

  private:
    Point3 origin;
    UnitVector3 direction;
  };
}  // namespace RayMan