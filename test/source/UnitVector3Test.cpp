#include <doctest/doctest.h>

#include <vector>

#include "TestUtils.hpp"
#include "UnitVector3.hpp"

using namespace RayMan;

TEST_CASE("UnitVector construction") {
  const auto CheckUnitVector = [](const Vector3& v, const Vector3& expected) {
    REQUIRE_MESSAGE(expected.length_square() == doctest::Approx(1),
                    "expected vector must have 1 length");
    const UnitVector3 u(v);
    TestUtils::CheckEqual(u.ToVector3(), expected);
  };

  SUBCASE("zero vector") { CHECK_THROWS(UnitVector3{Vector3::Zero}); }

  SUBCASE("axis-aligned") {
    CheckUnitVector({42, 0, 0}, {1, 0, 0});
    CheckUnitVector({-3.14, 0, 0}, {-1, 0, 0});

    CheckUnitVector({0, 5, 0}, {0, 1, 0});
    CheckUnitVector({0, -20, 0}, {0, -1, 0});

    CheckUnitVector({0, 0, 1.11}, {0, 0, 1});
    CheckUnitVector({0, 0, -99.99}, {0, 0, -1});
  }

  SUBCASE("arbitrary values") {
    CheckUnitVector({5.6, -7.9, 1.1}, {0.57460837, -0.81060823, 0.11286950});
    CheckUnitVector({3.7, -7.7, -7}, {0.33500972, -0.69718239, -0.63380218});
  }
}
