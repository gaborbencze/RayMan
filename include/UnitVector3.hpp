#include <Vector3.hpp>

namespace RayMan {
  class UnitVector3 {
  public:
    explicit UnitVector3(const Vector3& v);

    Vector3 operator*(const double t) const noexcept;

    double x() const noexcept { return vec.x(); }
    double y() const noexcept { return vec.y(); }
    double z() const noexcept { return vec.z(); }

  private:
    Vector3 vec;
  };

}  // namespace RayMan