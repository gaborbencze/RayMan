#pragma once

#include <Hittable/Hittable.hpp>
#include <Point3.hpp>
#include <memory>

namespace RayMan {
  class Material;

  class Sphere : public Hittable {
  public:
    Sphere(const Point3& center, double radius, std::shared_ptr<const Material> material);

  private:
    Point3 center;
    double radius;
    std::shared_ptr<const Material> material;

    std::optional<Hit> GetHitImpl(const Ray& ray, double distMin, double distMax) const override;
  };
}  // namespace RayMan