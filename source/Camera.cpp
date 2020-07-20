#include "Camera.hpp"

#include <cassert>
#include <cmath>

#include "MathUtils.hpp"
#include "RandomUtils.hpp"
#include "Ray.hpp"

namespace RayMan {

  Camera Camera::Create(Point3 cameraPosition, Point3 lookAt, double verticalFOV,
                        double aspectRatio, double aperture, double focusDistance) {
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
    const auto v = UnitVector3(Cross(w, u));

    const Vector3 horizontal = u * viewportWidth * focusDistance;
    const Vector3 vertical = v * viewportHeight * focusDistance;
    const Point3 lowerLeftCorner
        = cameraPosition - horizontal / 2 - vertical / 2 - w * focusDistance;

    return Camera{cameraPosition, horizontal, vertical, lowerLeftCorner, w, u, v, aperture / 2};
  }

  Ray Camera::GetRay(double s, double t) const {
    assert(0 <= s && s <= 1 && 0 <= t && t <= 1);
    const Vector3 rd = GetRandomVectorInUnitDisk() * lensRadius;
    const Vector3 offset = u * rd.x() + v * rd.y();

    const UnitVector3 direction(lowerLeftCorner + horizontal * s + vertical * t - origin - offset);
    return Ray(origin + offset, direction);
  }

  Camera::Camera(const Point3& origin, const Vector3& horizontal, const Vector3& vertical,
                 const Point3& lowerLeftCorner, const UnitVector3& w, const UnitVector3& u,
                 const UnitVector3& v, double lensRadius)
      : origin(origin),
        horizontal(horizontal),
        vertical(vertical),
        lowerLeftCorner(lowerLeftCorner),
        w(w),
        u(u),
        v(v),
        lensRadius(lensRadius) {}

}  // namespace RayMan