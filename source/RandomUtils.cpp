#include "RandomUtils.hpp"

#include <random>

#include "MathUtils.hpp"
#include "UnitVector3.hpp"
#include "Vector3.hpp"

namespace RayMan {

  double GetRandomDouble(double min, double max) {
    static thread_local std::mt19937 generator;  // NOLINT
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
  }

  UnitVector3 GetRandomUnitVector() {
    const auto a = GetRandomDouble(0, 2 * Pi);
    const auto z = GetRandomDouble(-1, 1);
    const auto r = std::sqrt(1 - z * z);
    return UnitVector3(r * std::cos(a), r * std::sin(a), z);
  }

  Vector3 GetRandomVectorInUnitSphere() {
    while (true) {
      const auto v
          = Vector3(GetRandomDouble(-1, 1), GetRandomDouble(-1, 1), GetRandomDouble(-1, 1));
      if (v.length_square() <= 1) {
        return v;
      }
    }
  }

  Vector3 GetRandomVectorInUnitDisk() {
    while (true) {
      const auto v = Vector3(GetRandomDouble(-1, 1), GetRandomDouble(-1, 1), 0);
      if (v.length_square() <= 1) {
        return v;
      }
    }
  }
}  // namespace RayMan