#include <Camera.hpp>
#include <Ray.hpp>
#include <cassert>
#include <cmath>

#include "MathUtils.hpp"

namespace RayMan {

  Camera Camera::Create(Point3 cameraPosition, Point3 lookAt, double verticalFOV,
                        double aspectRatio) {
    assert(verticalFOV > 0);
    assert(verticalFOV < 180);
    assert(aspectRatio > 0);

    const auto theta = DegreesToRadians(verticalFOV);
    const auto h = std::tan(theta / 2);
    const auto viewportHeight = 2 * h;
    const auto viewportWidth = aspectRatio * viewportHeight;

    const UnitVector3 up{0, 1, 0};

    const auto w = UnitVector3(cameraPosition - lookAt);
    const auto u = UnitVector3(Cross(up, w));
    const auto v = Cross(w, u);

    const Vector3 horizontal = u * viewportWidth;
    const Vector3 vertical = v * viewportHeight;
    const Point3 lowerLeftCorner = cameraPosition - horizontal / 2 - vertical / 2 - w.ToVector3();

    return Camera{cameraPosition, horizontal, vertical, lowerLeftCorner};
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