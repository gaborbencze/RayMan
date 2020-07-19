#pragma once

namespace RayMan {
  constexpr double Pi = 3.14159265358979323846;

  constexpr double DegreesToRadians(double degrees) { return degrees * Pi / 180; }

  constexpr double RadiansToDegrees(double radians) { return radians / Pi * 180; }
}  // namespace RayMan