#pragma once

namespace RayMan {
  class Vector3 {
  public:
    constexpr Vector3(double x, double y, double z) : _x(x), _y(y), _z(z) {}
    static const Vector3 Zero;

    constexpr double x() const noexcept { return _x; }
    constexpr double y() const noexcept { return _y; }
    constexpr double z() const noexcept { return _z; }

    double length() const noexcept;
    constexpr double length_square() const noexcept { return _x * _x + _y * _y + _z * _z; }

    constexpr Vector3& operator*=(const double t) noexcept {
      _x *= t;
      _y *= t;
      _z *= t;
      return *this;
    }

    constexpr Vector3& operator/=(const double t) {
      _x /= t;
      _y /= t;
      _z /= t;
      return *this;
    }

  private:
    double _x;
    double _y;
    double _z;
  };

  constexpr Vector3 operator*(const Vector3& v, const double t) {
    Vector3 result = v;
    result *= t;
    return result;
  }

  constexpr Vector3 operator/(const Vector3& v, const double t) {
    Vector3 result = v;
    result /= t;
    return result;
  }

}  // namespace RayMan