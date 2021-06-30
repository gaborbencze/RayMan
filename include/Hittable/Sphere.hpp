#pragma once

#include <memory>

#include "Hittable/Hittable.hpp"
#include "Point3.hpp"

namespace RayMan {
  class Material;

  class Sphere : public Hittable {
  public:
    Sphere(const Point3& center, double radius, std::shared_ptr<const Material> material);

    BoundingBox GetBoundingBox() const final;

  private:
    Point3 center;
    double radius;
    std::shared_ptr<const Material> material;

    std::optional<Hit> GetHitImpl(const Ray& ray, double distMin, double distMax) const final;
  };
}  // namespace RayMan