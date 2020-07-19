#include <doctest/doctest.h>

#include "Hit.hpp"
#include "Hittable/Sphere.hpp"
#include "Material/Lambertian.hpp"
#include "Ray.hpp"
#include "TestUtils.hpp"

using namespace RayMan;

TEST_CASE("Sphere hit test") {
  auto testMaterial = std::make_shared<Lambertian>(Color(1, 1, 1));

  SUBCASE("Sphere behind ray") {
    const RayMan::Sphere sphere({0, 0, 1}, 0.5, testMaterial);
    const RayMan::Ray ray({0, 0, 0}, {0, 0, -1});
    CHECK_FALSE(sphere.GetHit(ray));
  }

  SUBCASE("ray origin outside sphere") {
    const RayMan::Sphere sphere({0, 0, -1}, 0.5, testMaterial);
    const RayMan::Ray ray({0, 0, 0}, {0, 0, -1});

    const auto hit = sphere.GetHit(ray);
    REQUIRE(hit.has_value());
    TestUtils::CheckEqual(hit->point, {0, 0, -0.5});
    TestUtils::CheckEqual(hit->normal, {0, 0, 1});
    CHECK_EQ(hit->material, testMaterial.get());
  }

  SUBCASE("ray origin inside sphere") {
    const RayMan::Sphere sphere({0, 0, 0}, 0.5, testMaterial);
    const RayMan::Ray ray({0, 0, 0}, {0, 0, -1});

    const auto hit = sphere.GetHit(ray);
    REQUIRE(hit.has_value());
    TestUtils::CheckEqual(hit->point, {0, 0, -0.5});
    TestUtils::CheckEqual(hit->normal, {0, 0, 1});
    CHECK_EQ(hit->material, testMaterial.get());
  }
}