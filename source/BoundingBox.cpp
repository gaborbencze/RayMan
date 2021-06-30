#include "BoundingBox.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <utility>

#include "Ray.hpp"

namespace RayMan {
  BoundingBox BoundingBox::Merge(const BoundingBox& lhs, const BoundingBox& rhs) {
    const Point3 min{std::min(lhs.Min().x(), rhs.Min().x()), std::min(lhs.Min().y(), rhs.Min().y()),
                     std::min(lhs.Min().z(), rhs.Min().z())};

    const Point3 max{std::max(lhs.Max().x(), rhs.Max().x()), std::max(lhs.Max().y(), rhs.Max().y()),
                     std::max(lhs.Max().z(), rhs.Max().z())};

    return BoundingBox{min, max};
  }

  BoundingBox::BoundingBox(const Point3& min, const Point3& max) : min(min), max(max) {
    assert(min.x() <= max.x());
    assert(min.y() <= max.y());
    assert(min.z() <= max.z());
  }

  Point3 BoundingBox::Min() const { return min; }
  Point3 BoundingBox::Max() const { return max; }

  bool BoundingBox::IsHit(const Ray& ray, double distMin, double distMax) const {
    const auto HitsAxis = [&](auto GetCoord) {
      const auto inverseDirection = 1. / GetCoord(ray.GetDirection());
      auto t0 = (GetCoord(min) - GetCoord(ray.GetOrigin())) * inverseDirection;
      auto t1 = (GetCoord(max) - GetCoord(ray.GetOrigin())) * inverseDirection;
      if (inverseDirection < 0) {
        std::swap(t0, t1);
      }

      distMin = std::max(distMin, t0);
      distMax = std::min(distMax, t1);
      return distMax > distMin;
    };

    return HitsAxis([](const auto& v) { return v.x(); })
           && HitsAxis([](const auto& v) { return v.y(); })
           && HitsAxis([](const auto& v) { return v.z(); });
  }

}  // namespace RayMan