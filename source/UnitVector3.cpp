#include "UnitVector3.hpp"

#include <iostream>

namespace RayMan {

  UnitVector3::UnitVector3(const Vector3& v) : vec(v / v.length()) {}

  UnitVector3::UnitVector3(double x, double y, double z) : UnitVector3{Vector3(x, y, z)} {}

  Vector3 UnitVector3::operator*(const double t) const noexcept { return vec * t; }

  UnitVector3 UnitVector3::operator-() const noexcept { return UnitVector3(-vec); }

  double Dot(const UnitVector3& u, const UnitVector3& v) {
    return Dot(u.ToVector3(), v.ToVector3());
  }

  Vector3 Cross(const UnitVector3& u, const UnitVector3& v) {
    return Cross(u.ToVector3(), v.ToVector3());
  }

  UnitVector3 Reflect(const UnitVector3& v, const UnitVector3& normal) {
    return UnitVector3{v.ToVector3() - 2 * Dot(v, normal) * normal.ToVector3()};
  }

  Vector3 operator+(const Vector3& lhs, const UnitVector3& rhs) { return lhs + rhs.ToVector3(); }

  Vector3 operator+(const UnitVector3& lhs, const Vector3& rhs) { return lhs.ToVector3() + rhs; }

  Vector3 operator+(const UnitVector3& lhs, const UnitVector3& rhs) {
    return lhs.ToVector3() + rhs.ToVector3();
  }

  std::ostream& operator<<(std::ostream& os, const UnitVector3& v) {
    os << v.ToVector3();
    return os;
  }

}  // namespace RayMan