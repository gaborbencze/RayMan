#include "Material/Metal.hpp"

#include "Color.hpp"
#include "Hit.hpp"
#include "Ray.hpp"
#include "UnitVector3.hpp"

static RayMan::UnitVector3 Reflect(const RayMan::UnitVector3& v,
                                   const RayMan::UnitVector3& normal) {
  return RayMan::UnitVector3{v.ToVector3() - 2 * RayMan::Dot(v, normal) * normal.ToVector3()};
}

namespace RayMan {
  Metal::Metal(const Color& albedo) : albedo(albedo) {}

  std::optional<std::pair<Color, Ray>> Metal::Scatter(const Ray& ray, const Hit& hit) const {
    const auto reflected = Reflect(ray.GetDirection(), hit.normal);
    if (Dot(reflected, hit.normal) > 0) {
      return std::make_pair(albedo, Ray(hit.point, reflected));
    }
    return std::nullopt;
  }
}  // namespace RayMan