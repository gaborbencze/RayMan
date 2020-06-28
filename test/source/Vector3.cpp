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

TEST_CASE("Multiply by scalar (zero)") {
  Vector3 v(-5, 0, 314.15);

  v *= 0;

  CHECK(v.x() == doctest::Approx(0));
  CHECK(v.y() == doctest::Approx(0));
  CHECK(v.z() == doctest::Approx(0));
}

TEST_CASE("Multiply by scalar (one)") {
  const double x = 0;
  const double y = -1;
  const double z = 25.85;

  Vector3 v(x, y, z);

  v *= 1;

  CHECK(v.x() == doctest::Approx(x));
  CHECK(v.y() == doctest::Approx(y));
  CHECK(v.z() == doctest::Approx(z));
}

TEST_CASE("Multiply by scalar") {
  const double multiplier = 2.5;
  Vector3 v(1, 3.14, -42);

  v *= multiplier;

  CHECK(v.x() == doctest::Approx(2.5));
  CHECK(v.y() == doctest::Approx(7.85));
  CHECK(v.z() == doctest::Approx(-105));
}

TEST_CASE("Divide by scalar") {
  const double divisor = 2.5;
  Vector3 v(1, 3.14, -42);

  v /= divisor;

  CHECK(v.x() == doctest::Approx(0.4));
  CHECK(v.y() == doctest::Approx(1.256));
  CHECK(v.z() == doctest::Approx(-16.8));
}