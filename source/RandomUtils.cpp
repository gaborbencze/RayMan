#include "RandomUtils.hpp"

#include <random>

#include "MathUtils.hpp"
#include "UnitVector3.hpp"
#include "Vector3.hpp"

namespace RayMan {

  static std::mt19937& GetGenerator() {
    thread_local std::mt19937 generator;  // NOLINT
    return generator;
  }

  std::size_t GetRandomIndex(std::size_t max) {
    std::uniform_int_distribution<std::size_t> distribution(0, max);
    return distribution(GetGenerator());
  }

  double GetRandomDouble(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(GetGenerator());
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