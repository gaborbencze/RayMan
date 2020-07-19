#include "Color.hpp"

namespace RayMan {
  Color Interpolate(const Color& from, const Color& to, const double t) {
    return Color(from.r() + (to.r() - from.r()) * (1 - t), from.g() + (to.g() - from.g()) * (1 - t),
                 from.b() + (to.b() - from.b()) * (1 - t));
  }

  Color operator*(const Color& lhs, const Color& rhs) {
    return Color(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b());
  }
}  // namespace RayMan