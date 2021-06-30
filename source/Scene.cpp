#include "Scene.hpp"

#include <limits>

#include "Hit.hpp"
#include "Hittable/BVHNode.hpp"
#include "Hittable/Hittable.hpp"
#include "Ray.hpp"

static constexpr double EPS = 1E-5;

namespace RayMan {
  Scene::Scene(Hittables objects) : objects(std::make_unique<BVHNode>(std::move(objects))) {}

  Scene::Scene(Scene&&) noexcept = default;
  Scene::~Scene() = default;

  std::optional<Hit> Scene::GetHit(const Ray& ray) const {
    return objects->GetHit(ray, EPS, std::numeric_limits<double>::infinity());
  }
}  // namespace RayMan