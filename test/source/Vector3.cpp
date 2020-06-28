#include <doctest/doctest.h>

#include <Vector3.hpp>
#include <array>

using namespace RayMan;

TEST_CASE("Zero vector") {
  const Vector3& zero = Vector3::Zero;

  CHECK(zero.length() == 0);
  CHECK(zero.length_square() == 0);
  CHECK(zero.x() == 0);
  CHECK(zero.y() == 0);
  CHECK(zero.z() == 0);
}

TEST_CASE("Multiply vector by scalar") {
  const Vector3 v(-5, 0, 314.15);

  SUBCASE("multiply by zero") {
    const Vector3 res = v * 0;
    CHECK(res.x() == doctest::Approx(0));
    CHECK(res.y() == doctest::Approx(0));
    CHECK(res.z() == doctest::Approx(0));
  }

  SUBCASE("multiply by one") {
    const Vector3 res = v * 1;
    CHECK(res.x() == doctest::Approx(v.x()));
    CHECK(res.y() == doctest::Approx(v.y()));
    CHECK(res.z() == doctest::Approx(v.z()));
  }

  SUBCASE("arbitrary multiplier") {
    const double multiplier = 2.5;
    const Vector3 res = v * multiplier;

    CHECK(res.x() == doctest::Approx(-12.5));
    CHECK(res.y() == doctest::Approx(0));
    CHECK(res.z() == doctest::Approx(785.375));
  }
}

TEST_CASE("Divide vector by scalar") {
  const Vector3 v(1, 3.14, -42);

  SUBCASE("divide by zero") { CHECK_THROWS(v / 0); }

  SUBCASE("divide by one") {
    const Vector3 res = v / 1;
    CHECK(res.x() == doctest::Approx(v.x()));
    CHECK(res.y() == doctest::Approx(v.y()));
    CHECK(res.z() == doctest::Approx(v.z()));
  }

  SUBCASE("arbitrary divisor") {
    const double divisor = 2.5;
    const Vector3 res = v / divisor;
    CHECK(res.x() == doctest::Approx(0.4));
    CHECK(res.y() == doctest::Approx(1.256));
    CHECK(res.z() == doctest::Approx(-16.8));
  }
}

TEST_CASE("Vector dot product") {
  const auto CheckDotProduct = [](const Vector3& u, const Vector3& v, const double expected) {
    CAPTURE(u);
    CAPTURE(v);
    CHECK(Dot(u, v) == doctest::Approx(expected));
    CHECK(Dot(v, u) == doctest::Approx(expected));
  };

  SUBCASE("zero vector") {
    CheckDotProduct(Vector3::Zero, Vector3::Zero, 0);
    CheckDotProduct({1, 2, 3}, Vector3::Zero, 0);
  }

  SUBCASE("orthogonal vectors") {
    const Vector3 x{1, 0, 0};
    const Vector3 y{0, 3.14, 0};
    const Vector3 z{0, 0, -5};

    CheckDotProduct(x, y, 0);
    CheckDotProduct(x, z, 0);
    CheckDotProduct(y, z, 0);

    CheckDotProduct({1, 1, 0}, {-1, 1, 0}, 0);
  }

  SUBCASE("self dot product") {
    const auto vectors = std::array{Vector3{1, 2, 3}, Vector3{9, 8, 7}};

    for (const auto& vector : vectors) {
      CheckDotProduct(vector, vector, vector.length_square());
    }
  }

  SUBCASE("arbitrary test cases") {
    CheckDotProduct({1, 2, 3}, {9, 8, 7}, 46);
    CheckDotProduct({-10, 3, 1}, {6, -6, -7}, -85);
    CheckDotProduct({7.7, 0.8, 0.1}, {-9.8, 6.1, 5.3}, -70.05);
  }
}

TEST_CASE("Vector cross product") {
  const auto CheckCrossProduct = [](const Vector3& u, const Vector3& v, const Vector3& expected) {
    CAPTURE(u);
    CAPTURE(v);
    CAPTURE(expected);

    const auto result = Cross(u, v);
    CHECK(result.x() == doctest::Approx(expected.x()));
    CHECK(result.y() == doctest::Approx(expected.y()));
    CHECK(result.z() == doctest::Approx(expected.z()));

    const auto result_reversed = Cross(v, u);
    CHECK(result_reversed.x() == doctest::Approx(-expected.x()));
    CHECK(result_reversed.y() == doctest::Approx(-expected.y()));
    CHECK(result_reversed.z() == doctest::Approx(-expected.z()));
  };

  SUBCASE("zero vector") {
    CheckCrossProduct(Vector3::Zero, Vector3::Zero, Vector3::Zero);
    CheckCrossProduct({1, 2, 3}, Vector3::Zero, Vector3::Zero);
  }

  SUBCASE("axis-aligned unit vectors") {
    const Vector3 x{1, 0, 0};
    const Vector3 y{0, 1, 0};
    const Vector3 z{0, 0, 1};

    CheckCrossProduct(x, y, z);
    CheckCrossProduct(x, z, -y);
    CheckCrossProduct(y, z, x);
  }

  SUBCASE("arbitrary test cases") {
    CheckCrossProduct({1, 2, 3}, {9, 8, 7}, {-10, 20, -10});
    CheckCrossProduct({-10, 3, 1}, {6, -6, -7}, {-15, -64, 42});
    CheckCrossProduct({7.7, 0.8, 0.1}, {-9.8, 6.1, 5.3}, {3.63, -41.79, 54.81});
  }
}