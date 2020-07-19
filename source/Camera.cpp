#include <Camera.hpp>
#include <Ray.hpp>
#include <cassert>
#include <cmath>

#include "MathUtils.hpp"

namespace RayMan {

  static constexpr double FocalLength = 1;

  Camera Camera::Create(double verticalFOV, double aspectRatio) {
    assert(verticalFOV > 0);
    assert(aspectRatio > 0);

    const auto theta = DegreesToRadians(verticalFOV);
    const auto h = std::tan(theta / 2);
    const auto viewportHeight = 2 * h;
    const auto viewportWidth = aspectRatio * viewportHeight;

    const Point3 origin{0, 0, 0};
    const Vector3 horizontal{viewportWidth, 0, 0};
    const Vector3 vertical{0, viewportHeight, 0};
    const Point3 lowerLeftCorner
        = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, FocalLength);

    return Camera{origin, horizontal, vertical, lowerLeftCorner};
  }

  Ray Camera::GetRay(double u, double v) const {
    assert(0 <= u && u <= 1 && 0 <= v && v <= 1);
    const UnitVector3 direction(lowerLeftCorner + horizontal * u + vertical * v - origin);
    return Ray(origin, direction);
  }
  Camera::Camera(Point3 origin, Vector3 horizontal, Vector3 vertical, Point3 lowerLeftCorner)
      : origin(origin),
        horizontal(horizontal),
        vertical(vertical),
        lowerLeftCorner(lowerLeftCorner) {}

}  // namespace RayMan