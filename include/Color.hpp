#pragma once

namespace RayMan {
  class Color {
  public:
    constexpr Color(double r, double g, double b) noexcept : _r(r), _g(g), _b(b) {}

    constexpr double r() const noexcept { return _r; }
    constexpr double g() const noexcept { return _g; }
    constexpr double b() const noexcept { return _b; }

  private:
    double _r;
    double _g;
    double _b;
  };
}  // namespace RayMan