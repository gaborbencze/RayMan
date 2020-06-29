#include <doctest/doctest.h>

#include <Hit.hpp>
#include <Hittable/Sphere.hpp>
#include <Ray.hpp>
#include <TestUtils.hpp>

using namespace RayMan;

TEST_CASE("Sphere hit test") {
  SUBCASE("Sphere behind ray") {
    const RayMan::Sphere sphere({0, 0, 1}, 0.5);
    const RayMan::Ray ray({0, 0, 0}, {0, 0, -1});
    CHECK_FALSE(sphere.GetHit(ray));
  }

  SUBCASE("ray origin outside sphere") {
    const RayMan::Sphere sphere({0, 0, -1}, 0.5);
    const RayMan::Ray ray({0, 0, 0}, {0, 0, -1});

    const auto hit = sphere.GetHit(ray);
    REQUIRE(hit.has_value());
    TestUtils::CheckEqual(hit->point, {0, 0, -0.5});
    TestUtils::CheckEqual(hit->normal, {0, 0, 1});
  }

  SUBCASE("ray origin inside sphere") {
    const RayMan::Sphere sphere({0, 0, 0}, 0.5);
    const RayMan::Ray ray({0, 0, 0}, {0, 0, -1});

    const auto hit = sphere.GetHit(ray);
    REQUIRE(hit.has_value());
    TestUtils::CheckEqual(hit->point, {0, 0, -0.5});
    TestUtils::CheckEqual(hit->normal, {0, 0, 1});
  }
}