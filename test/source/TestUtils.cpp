#include <doctest/doctest.h>

#include <Point3.hpp>
#include <TestUtils.hpp>
#include <UnitVector3.hpp>
#include <Vector3.hpp>

namespace RayMan {

  namespace TestUtils {
    void CheckEqual(const Vector3& u, const Vector3& v) {
      CAPTURE(u);
      CAPTURE(v);

      CHECK(u.x() == doctest::Approx(v.x()));
      CHECK(u.y() == doctest::Approx(v.y()));
      CHECK(u.z() == doctest::Approx(v.z()));
    }
    void CheckEqual(const UnitVector3& u, const UnitVector3& v) {
      CAPTURE(u);
      CAPTURE(v);

      CHECK(u.x() == doctest::Approx(v.x()));
      CHECK(u.y() == doctest::Approx(v.y()));
      CHECK(u.z() == doctest::Approx(v.z()));
    }
    void CheckEqual(const Point3& u, const Point3& v) {
      CAPTURE(u);
      CAPTURE(v);

      CHECK(u.x() == doctest::Approx(v.x()));
      CHECK(u.y() == doctest::Approx(v.y()));
      CHECK(u.z() == doctest::Approx(v.z()));
    }
  }  // namespace TestUtils
}  // namespace RayMan
