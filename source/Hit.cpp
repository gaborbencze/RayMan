#include "Hit.hpp"

#include "Ray.hpp"

namespace RayMan {

  static bool IsFrontFace(const UnitVector3& outwardNormal, const UnitVector3& rayDirection) {
    return Dot(outwardNormal, rayDirection) < 0;
  }

  static UnitVector3 GetFaceNormal(const UnitVector3& outwardNormal, const bool isFrontFace) {
    if (isFrontFace) {
      return outwardNormal;
    }
    return -outwardNormal;
  }

  Hit::Hit(const Point3& point, const UnitVector3& outwardNormal, const Ray& ray,
           const Material* material)
      : point(point),
        isFrontFace(IsFrontFace(outwardNormal, ray.GetDirection())),
        normal(GetFaceNormal(outwardNormal, isFrontFace)),
        material(material) {}
}  // namespace RayMan