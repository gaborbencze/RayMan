#include <Hit.hpp>
#include <Hittable/Sphere.hpp>
#include <Ray.hpp>
#include <cmath>

namespace RayMan {
  Sphere::Sphere(const Point3& center, double radius) : center(center), radius(radius) {}

  std::optional<Hit> Sphere::GetHitImpl(const Ray& ray, double distMin, double distMax) const {
    using RayMan::Dot;

    const auto centerToOrigin = ray.GetOrigin() - center;
    const double halfB = Dot(centerToOrigin, ray.GetDirection().ToVector3());
    const double c = centerToOrigin.length_square() - radius * radius;
    const double discriminant = halfB * halfB - c;
    if (discriminant < 0) {
      return std::nullopt;
    }

    const auto root = std::sqrt(discriminant);

    const auto CalcHit = [this, &ray](const double distance) {
      const Point3 point = ray.At(distance);
      const UnitVector3 normal = UnitVector3(point - this->center);
      return Hit(point, normal, ray);
    };

    if (double t = (-halfB - root); distMin < t && t < distMax) {
      return CalcHit(t);
    } else if (double t2 = (-halfB + root); distMin < t2 && t2 < distMax) {
      return CalcHit(t2);
    }
    return std::nullopt;
  }
}  // namespace RayMan