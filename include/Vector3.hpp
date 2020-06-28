#pragma once

namespace RayMan {
  class Vector3 {
  public:
    constexpr Vector3(double x, double y, double z) : _x(x), _y(y), _z(z) {}
    static const Vector3 Zero;

    constexpr double x() const { return _x; }
    constexpr double y() const { return _y; }
    constexpr double z() const { return _z; }

    constexpr void x(double newX) { _x = newX; }
    constexpr void y(double newY) { _y = newY; }
    constexpr void z(double newZ) { _z = newZ; }

    double length() const;
    constexpr double length_square() const { return _x * _x + _y * _y + _z * _z; }

  private:
    double _x;
    double _y;
    double _z;
  };

  constexpr Vector3 operator*(const Vector3& v, const double t) {
    return Vector3(v.x() * t, v.y() * t, v.z() * t);
  }

  constexpr Vector3 operator/(const Vector3& v, const double t) {
    return Vector3(v.x() / t, v.y() / t, v.z() / t);
  }

}  // namespace RayMan