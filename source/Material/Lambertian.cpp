#include "Material/Lambertian.hpp"

#include "Color.hpp"
#include "Hit.hpp"
#include "RandomUtils.hpp"
#include "Ray.hpp"

namespace RayMan {

  Lambertian::Lambertian(const Color& albedo) : albedo(albedo) {}

  std::optional<std::pair<Color, Ray>> Lambertian::Scatter(const Ray& /*ray*/, const Hit& hit) {
    const auto reflectionRayDirection
        = RayMan::UnitVector3(hit.normal.ToVector3() + RayMan::GetRandomUnitVector().ToVector3());

    return std::make_pair(albedo, Ray(hit.point, reflectionRayDirection));
  }

}  // namespace RayMan