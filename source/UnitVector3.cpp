#include <UnitVector3.hpp>
#include <iostream>

namespace RayMan {

  UnitVector3::UnitVector3(const Vector3& v) : vec(v / v.length()) {}

  Vector3 UnitVector3::operator*(const double t) const noexcept { return vec * t; }

  std::ostream& operator<<(std::ostream& os, const UnitVector3& v) {
    os << v.ToVector3();
    return os;
  }

}  // namespace RayMan