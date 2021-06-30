#pragma once

#include <cstddef>

namespace RayMan {
  class Vector3;
  class UnitVector3;

  std::size_t GetRandomIndex(std::size_t max);
  double GetRandomDouble(double min, double max);

  UnitVector3 GetRandomUnitVector();
  Vector3 GetRandomVectorInUnitSphere();
  Vector3 GetRandomVectorInUnitDisk();

}  // namespace RayMan