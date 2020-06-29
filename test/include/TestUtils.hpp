#pragma once

namespace RayMan {
  class Vector3;
  class UnitVector3;
  class Point3;

  namespace TestUtils {
    void CheckEqual(const Vector3& u, const Vector3& v);
    void CheckEqual(const UnitVector3& u, const UnitVector3& v);
    void CheckEqual(const Point3& u, const Point3& v);
  }  // namespace TestUtils
}  // namespace RayMan
