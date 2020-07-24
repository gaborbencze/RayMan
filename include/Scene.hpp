#pragma once

#include <memory>
#include <optional>
#include <vector>

namespace RayMan {
  class Hittable;
  struct Hit;
  class Ray;

  class Scene {
  public:
    Scene();
    Scene(Scene&&) noexcept;
    ~Scene();

    void Add(std::unique_ptr<Hittable> object);
    std::optional<Hit> GetHit(const Ray& ray) const;

  private:
    std::vector<std::unique_ptr<Hittable>> objects;
  };
}  // namespace RayMan