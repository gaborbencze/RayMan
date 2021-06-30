#include "Hittable/Sphere.hpp"

#include <cassert>
#include <cmath>

#include "BoundingBox.hpp"
#include "Hit.hpp"
#include "Ray.hpp"

namespace RayMan {
  Sphere::Sphere(const Point3& center, double radius, std::shared_ptr<const Material> material)
      : center(center), radius(radius), material(std::move(material)) {
    assert(radius > 0);
  }

  BoundingBox Sphere::GetBoundingBox() const {
    const Vector3 diagonal{radius, radius, radius};
    return BoundingBox(center - diagonal, center + diagonal);
  }

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
      return Hit(point, normal, ray, material.get());
    };

    if (double t = (-halfB - root); distMin < t && t < distMax) {
      return CalcHit(t);
    }
    if (double t2 = (-halfB + root); distMin < t2 && t2 < distMax) {
      return CalcHit(t2);
    }
    return std::nullopt;
  }
}  // namespace RayMan