#pragma once

#include <iosfwd>
#include <stdexcept>

namespace RayMan {
  class Vector3 {
  public:
    constexpr Vector3(double x, double y, double z) noexcept : _x(x), _y(y), _z(z) {}
    static const Vector3 Zero;

    constexpr double x() const noexcept { return _x; }
    constexpr double y() const noexcept { return _y; }
    constexpr double z() const noexcept { return _z; }

    double length() const noexcept;
    constexpr double length_square() const noexcept { return _x * _x + _y * _y + _z * _z; }

    constexpr Vector3& operator+=(const Vector3& v) noexcept {
      _x += v.x();
      _y += v.y();
      _z += v.z();
      return *this;
    }

    constexpr Vector3 operator-() const noexcept { return Vector3(-_x, -_y, -_z); }

    constexpr Vector3& operator*=(const double t) noexcept {
      _x *= t;
      _y *= t;
      _z *= t;
      return *this;
    }

    constexpr Vector3& operator/=(const double t) {
      if (t == 0) {
        throw std::invalid_argument("t");
      }

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

  constexpr Vector3 operator+(const Vector3& u, const Vector3& v) noexcept {
    Vector3 result = u;
    result += v;
    return result;
  }

  constexpr Vector3 operator-(const Vector3& u, const Vector3& v) noexcept { return u + (-v); }

  constexpr Vector3 operator*(const Vector3& v, const double t) noexcept {
    Vector3 result = v;
    result *= t;
    return result;
  }

  constexpr Vector3 operator*(const double t, const Vector3& v) noexcept { return v * t; }

  constexpr Vector3 operator/(const Vector3& v, const double t) {
    Vector3 result = v;
    result /= t;
    return result;
  }

  double Dot(const Vector3& u, const Vector3& v);
  Vector3 Cross(const Vector3& u, const Vector3& v);

  std::ostream& operator<<(std::ostream& os, const Vector3& v);
}  // namespace RayMan
