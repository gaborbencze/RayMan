#pragma once

#include <Hittable/Hittable.hpp>
#include <Point3.hpp>

namespace RayMan {
  class Sphere : public Hittable {
  public:
    Sphere(const Point3& center, double radius);

  private:
    Point3 center;
    double radius;

    std::optional<Hit> GetHitImpl(const Ray& ray, double distMin, double distMax) const override;
  };
}  // namespace RayMan