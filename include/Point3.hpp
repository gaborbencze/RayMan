#pragma once

#include "Vector3.hpp"

namespace RayMan {
  class Point3 {
  public:
    constexpr Point3(double x, double y, double z) noexcept : vec(x, y, z) {}

    constexpr double x() const noexcept { return vec.x(); }
    constexpr double y() const noexcept { return vec.y(); }
    constexpr double z() const noexcept { return vec.z(); }

    constexpr Point3& operator+=(const Vector3& offset) noexcept {
      vec += offset;
      return *this;
    }

  private:
    Vector3 vec;
  };

  constexpr Point3 operator+(const Point3& p, const Vector3& v) noexcept {
    Point3 result = p;
    result += v;
    return result;
  }

  constexpr Point3 operator-(const Point3& p, const Vector3& v) noexcept {
    Point3 result = p;
    result += (-v);
    return result;
  }

  constexpr Vector3 operator-(const Point3& lhs, const Point3& rhs) noexcept {
    return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
  }

}  // namespace RayMan