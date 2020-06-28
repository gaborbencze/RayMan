#include <Vector3.hpp>
#include <cmath>

namespace RayMan {

  const Vector3 Vector3::Zero = Vector3(0, 0, 0);

  double Vector3::length() const noexcept { return std::sqrt(length_square()); }

}  // namespace RayMan