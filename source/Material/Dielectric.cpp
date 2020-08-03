#include "Material/Dielectric.hpp"

#include <algorithm>
#include <cmath>

#include "Color.hpp"
#include "Hit.hpp"
#include "RandomUtils.hpp"
#include "Ray.hpp"
#include "UnitVector3.hpp"

namespace RayMan {

  namespace {
    UnitVector3 Refract(const UnitVector3& uv, const UnitVector3& normal, double etaiOverEtat) {
      using std::sqrt;

      const auto cosTheta = Dot(-uv, normal);
      Vector3 r_out_perp = etaiOverEtat * (uv + normal * cosTheta);
      Vector3 r_out_parallel = normal * -sqrt(std::abs(1.0 - r_out_perp.length_square()));
      return UnitVector3(r_out_perp + r_out_parallel);
    }

    double SchlickApproximation(double cosine, double refractiveIndex) {
      const auto r0 = std::pow((1 - refractiveIndex) / (1 + refractiveIndex), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
      return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
  }  // namespace

  Dielectric::Dielectric(double refractiveIndex) : refractiveIndex(refractiveIndex) {}

  std::optional<std::pair<Color, Ray>> Dielectric::Scatter(const Ray& ray, const Hit& hit) const {
    const auto attenuation = Color::White();
    const double etaiOverEtat = hit.isFrontFace ? (1 / refractiveIndex) : refractiveIndex;

    const double cosTheta = std::min(Dot(-ray.GetDirection(), hit.normal), 1.0);
    const double sinTheta = std::sqrt(1 - cosTheta * cosTheta);

    const double reflectionProbability = SchlickApproximation(cosTheta, etaiOverEtat);

    const bool canRefract = etaiOverEtat * sinTheta <= 1;
    const bool shouldReflect = RayMan::GetRandomDouble(0, 1) < reflectionProbability;

    if (!canRefract || shouldReflect) {
      const auto reflectedDirection = Reflect(ray.GetDirection(), hit.normal);
      const Ray reflectedRay(hit.point, reflectedDirection);
      return std::make_pair(attenuation, reflectedRay);
    }

    const auto refractedDirection = Refract(ray.GetDirection(), hit.normal, etaiOverEtat);
    const Ray scatteredRay(hit.point, refractedDirection);
    return std::make_pair(attenuation, scatteredRay);
  }

}  // namespace RayMan