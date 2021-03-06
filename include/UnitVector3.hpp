#pragma once

#include "Vector3.hpp"

namespace RayMan {
  class UnitVector3 {
  public:
    explicit UnitVector3(const Vector3& v);
    UnitVector3(double x, double y, double z);

    double x() const noexcept { return vec.x(); }
    double y() const noexcept { return vec.y(); }
    double z() const noexcept { return vec.z(); }

    Vector3 ToVector3() const noexcept { return vec; }

    Vector3 operator*(const double t) const noexcept;
    UnitVector3 operator-() const noexcept;

  private:
    Vector3 vec;
  };

  double Dot(const UnitVector3& u, const UnitVector3& v);
  Vector3 Cross(const UnitVector3& u, const UnitVector3& v);

  UnitVector3 Reflect(const UnitVector3& v, const UnitVector3& normal);

  Vector3 operator+(const Vector3& lhs, const UnitVector3& rhs);
  Vector3 operator+(const UnitVector3& lhs, const Vector3& rhs);
  Vector3 operator+(const UnitVector3& lhs, const UnitVector3& rhs);

  std::ostream& operator<<(std::ostream& os, const UnitVector3& v);
}  // namespace RayMan