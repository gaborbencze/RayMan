#pragma once

#include "Color.hpp"
#include "Material/Material.hpp"

namespace RayMan {
  class Metal : public Material {
  public:
    Metal(const Color& albedo);

    std::optional<std::pair<Color, Ray>> Scatter(const Ray& ray, const Hit& hit) const override;

  private:
    Color albedo;
  };
}  // namespace RayMan