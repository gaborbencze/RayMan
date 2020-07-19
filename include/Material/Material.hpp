#pragma once

#include <optional>
#include <utility>

namespace RayMan {
  struct Hit;
  class Ray;
  class Color;

  class Material {
  public:
    virtual ~Material();

    virtual std::optional<std::pair<Color, Ray>> Scatter(const Ray& ray, const Hit& hit);
  };
}  // namespace RayMan