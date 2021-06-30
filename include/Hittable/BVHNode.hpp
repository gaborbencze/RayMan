#pragma once

#include <array>
#include <memory>
#include <vector>

#include "BoundingBox.hpp"
#include "Hittable/Hittable.hpp"

namespace RayMan {
  class BVHNode final : public Hittable {
  public:
    BVHNode(Hittables objects);

    BoundingBox GetBoundingBox() const final;

  private:
    std::optional<Hit> GetHitImpl(const Ray& ray, double distMin, double distMax) const final;

    std::array<std::unique_ptr<Hittable>, 2> children;
    BoundingBox boundingBox;
  };
}  // namespace RayMan