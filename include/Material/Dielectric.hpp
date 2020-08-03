#pragma once

#include "Material/Material.hpp"

namespace RayMan {
  class Dielectric : public Material {
  public:
    Dielectric(double refractiveIndex);

    std::optional<std::pair<Color, Ray>> Scatter(const Ray& ray, const Hit& hit) const override;

  private:
    double refractiveIndex;
  };
}  // namespace RayMan