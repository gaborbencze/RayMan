#include "Vector3.hpp"

#include <cmath>
#include <iostream>

namespace RayMan {

  const Vector3 Vector3::Zero = Vector3(0, 0, 0);

  double Vector3::length() const noexcept { return std::sqrt(length_square()); }

  double Dot(const Vector3& u, const Vector3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
  }

  Vector3 Cross(const Vector3& u, const Vector3& v) {
    const double x = u.y() * v.z() - u.z() * v.y();
    const double y = u.z() * v.x() - u.x() * v.z();
    const double z = u.x() * v.y() - u.y() * v.x();
    return Vector3(x, y, z);
  }

  std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "<" << v.x() << "," << v.y() << "," << v.z() << ">";
    return os;
  }
}  // namespace RayMan
