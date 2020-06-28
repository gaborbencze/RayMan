#include <Vector3.hpp>

namespace RayMan {
  class UnitVector3 {
  public:
    explicit UnitVector3(const Vector3& v);

    Vector3 operator*(const double t) const noexcept;

    double x() const noexcept { return vec.x(); }
    double y() const noexcept { return vec.y(); }
    double z() const noexcept { return vec.z(); }

    Vector3 ToVector3() const noexcept { return vec; }

  private:
    Vector3 vec;
  };

  std::ostream& operator<<(std::ostream& os, const UnitVector3& v);
}  // namespace RayMan