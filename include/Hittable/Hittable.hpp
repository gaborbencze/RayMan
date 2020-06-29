#pragma once

#include <optional>

namespace RayMan {
  struct Hit;
  class Ray;

  class Hittable {
  public:
    virtual ~Hittable() = default;

    std::optional<Hit> GetHit(const Ray& ray) const;
    std::optional<Hit> GetHit(const Ray& ray, double distMin, double distMax) const;

  private:
    virtual std::optional<Hit> GetHitImpl(const Ray& ray, double distMin, double distMax) const = 0;
  };
}  // namespace RayMan