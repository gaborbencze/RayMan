#include "Ray.hpp"

namespace RayMan {
  Ray::Ray(const Point3& origin, const UnitVector3& direction)
      : origin(origin), direction(direction) {}

  Point3 Ray::GetOrigin() const { return origin; }
  UnitVector3 Ray::GetDirection() const { return direction; }

  Point3 Ray::At(const double distance) const { return GetOrigin() + GetDirection() * distance; }
}  // namespace RayMan