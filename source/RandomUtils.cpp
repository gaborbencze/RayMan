#include "RandomUtils.hpp"

#include <random>

#include "UnitVector3.hpp"

static constexpr double Pi = 3.14159265358979323846;

namespace RayMan {

  double GetRandomDouble(double min, double max) {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
  }

  UnitVector3 GetRandomUnitVector() {
    const auto a = GetRandomDouble(0, 2 * Pi);
    const auto z = GetRandomDouble(-1, 1);
    const auto r = std::sqrt(1 - z * z);
    return UnitVector3(r * std::cos(a), r * std::sin(a), z);
  }
}  // namespace RayMan