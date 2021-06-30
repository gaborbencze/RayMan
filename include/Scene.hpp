#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "Hittable/Hittable.hpp"

namespace RayMan {
  class Hittable;
  struct Hit;
  class Ray;

  class Scene {
  public:
    explicit Scene(Hittables objects);
    Scene(Scene&&) noexcept;
    ~Scene();

    std::optional<Hit> GetHit(const Ray& ray) const;

  private:
    std::unique_ptr<Hittable> objects;
  };
}  // namespace RayMan