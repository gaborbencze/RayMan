#include <doctest/doctest.h>

#include <Vector3.hpp>

using namespace RayMan;

TEST_CASE("Zero vector") {
  const Vector3& zero = Vector3::Zero;

  CHECK(zero.length() == 0);
  CHECK(zero.length_square() == 0);
  CHECK(zero.x() == 0);
  CHECK(zero.y() == 0);
  CHECK(zero.z() == 0);
}