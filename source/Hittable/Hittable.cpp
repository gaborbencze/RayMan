#include "Hittable/Hittable.hpp"

#include <limits>

#include "Hit.hpp"

namespace RayMan {
  std::optional<Hit> Hittable::GetHit(const Ray& ray) const {
    return GetHitImpl(ray, 0, std::numeric_limits<double>::max());
  }

  std::optional<Hit> Hittable::GetHit(const Ray& ray, double distMin, double distMax) const {
    return GetHitImpl(ray, distMin, distMax);
  }
}  // namespace RayMan