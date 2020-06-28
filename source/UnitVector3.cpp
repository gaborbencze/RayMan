#include <UnitVector3.hpp>

namespace RayMan {

  UnitVector3::UnitVector3(const Vector3& v) : vec(v / v.length()) {}

  Vector3 UnitVector3::operator*(const double t) const noexcept { return vec * t; }

}  // namespace RayMan