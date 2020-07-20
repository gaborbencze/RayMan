#pragma once

namespace RayMan {
  class Vector3;
  class UnitVector3;

  double GetRandomDouble(double min, double max);

  UnitVector3 GetRandomUnitVector();
  Vector3 GetRandomVectorInUnitSphere();
  Vector3 GetRandomVectorInUnitDisk();

}  // namespace RayMan