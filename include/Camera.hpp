#pragma once

#include "Point3.hpp"
#include "UnitVector3.hpp"
#include "Vector3.hpp"

namespace RayMan {

  class Ray;

  class Camera {
  public:
    static Camera Create(Point3 cameraPosition, Point3 lookAt, double verticalFOV,
                         double aspectRatio, double aperture, double focusDistance);

    Ray GetRay(double u, double v) const;

  private:
    Camera(const Point3& origin, const Vector3& horizontal, const Vector3& vertical,
           const Point3& lowerLeftCorner, const UnitVector3& w, const UnitVector3& u,
           const UnitVector3& v, double lensRadius);

    Point3 origin;
    Vector3 horizontal;
    Vector3 vertical;
    Point3 lowerLeftCorner;

    UnitVector3 w;
    UnitVector3 u;
    UnitVector3 v;

    double lensRadius;
  };
}  // namespace RayMan