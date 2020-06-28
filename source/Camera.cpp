#include <Camera.hpp>
#include <Ray.hpp>
#include <cassert>

namespace RayMan {

  static constexpr double AspectRatio = 16.0 / 9.0;
  static constexpr double ViewportHeight = 2;
  static constexpr double ViewportWidth = ViewportHeight * AspectRatio;
  static constexpr double FocalLength = 1;

  Camera::Camera()
      : origin(0, 0, 0),
        horizontal(ViewportWidth, 0, 0),
        vertical(0, ViewportHeight, 0),
        lowerLeftCorner(origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, FocalLength)) {}

  Ray Camera::GetRay(double u, double v) const {
    assert(0 <= u && u <= 1 && 0 <= v && v <= 1);
    const UnitVector3 direction(lowerLeftCorner + horizontal * u + vertical * v - origin);
    return Ray(origin, direction);
  }

}  // namespace RayMan