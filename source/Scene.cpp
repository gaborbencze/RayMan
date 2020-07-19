#include "Scene.hpp"

#include <limits>

#include "Hit.hpp"
#include "Hittable/Hittable.hpp"
#include "Ray.hpp"

static constexpr double EPS = 1E-5;

namespace RayMan {
  Scene::Scene() = default;
  Scene::Scene(Scene&&) = default;
  Scene::~Scene() = default;

  void Scene::Add(std::unique_ptr<Hittable> object) { objects.push_back(std::move(object)); }

  std::optional<Hit> Scene::GetHit(const Ray& ray) const {
    std::optional<Hit> closestHit;
    double distanceToClosest = std::numeric_limits<double>::infinity();

    for (const auto& object : objects) {
      if (const auto hit = object->GetHit(ray, EPS, distanceToClosest)) {
        closestHit = hit;
        distanceToClosest = ((hit->point) - ray.GetOrigin()).length();
      }
    }

    return closestHit;
  }
}  // namespace RayMan