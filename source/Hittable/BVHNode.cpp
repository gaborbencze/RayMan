#include "Hittable/BVHNode.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>

#include "Hit.hpp"
#include "RandomUtils.hpp"
#include "Ray.hpp"

namespace RayMan {

  namespace {
    std::unique_ptr<Hittable> MakeNode(Hittables objects) {
      if (objects.size() == 1) {
        return std::move(objects.front());
      }
      return std::make_unique<BVHNode>(std::move(objects));
    }

    std::function<double(const Hittable&)> GetRandomAxisGetterFunction() {
      std::array<std::function<double(const Hittable&)>, 3> functions{
          [](const Hittable& e) { return e.GetBoundingBox().Min().x(); },
          [](const Hittable& e) { return e.GetBoundingBox().Min().y(); },
          [](const Hittable& e) { return e.GetBoundingBox().Min().z(); }};

      return functions.at(GetRandomIndex(functions.size() - 1));
    }

    std::pair<Hittables, Hittables> SplitVector(Hittables vec) {
      std::pair<Hittables, Hittables> result;

      const auto mid = [&vec] {
        auto mid = vec.begin();
        std::advance(mid, std::distance(vec.begin(), vec.end()) / 2);
        return mid;
      }();

      std::move(vec.begin(), mid, std::back_inserter(result.first));
      std::move(mid, vec.end(), std::back_inserter(result.second));

      return result;
    }

    std::array<std::unique_ptr<Hittable>, 2> SplitObjects(Hittables objects) {
      assert(objects.size() >= 2);

      const auto comparator
          = [GetAxis = GetRandomAxisGetterFunction()](const std::unique_ptr<Hittable>& lhs,
                                                      const std::unique_ptr<Hittable>& rhs) {
              return GetAxis(*lhs) < GetAxis(*rhs);
            };

      std::sort(std::begin(objects), std::end(objects), comparator);
      auto [leftHittables, rightHittables] = SplitVector(std::move(objects));
      return {MakeNode(std::move(leftHittables)), MakeNode(std::move(rightHittables))};
    }
  }  // namespace

  BVHNode::BVHNode(Hittables objects)
      : children(SplitObjects(std::move(objects))),
        boundingBox(
            BoundingBox::Merge(children[0]->GetBoundingBox(), children[1]->GetBoundingBox())) {}

  BoundingBox BVHNode::GetBoundingBox() const { return boundingBox; }

  std::optional<Hit> BVHNode::GetHitImpl(const Ray& ray, double distMin, double distMax) const {
    if (!boundingBox.IsHit(ray, distMin, distMax)) {
      return std::nullopt;
    }

    std::optional<Hit> closestHit;
    double distanceToClosest = distMax;

    for (const auto& child : children) {
      if (const auto hit = child->GetHit(ray, distMin, distanceToClosest)) {
        closestHit = hit;
        distanceToClosest = ((hit->point) - ray.GetOrigin()).length();
      }
    }
    return closestHit;
  }

}  // namespace RayMan