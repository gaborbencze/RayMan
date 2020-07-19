#include <Hit.hpp>
#include <Ray.hpp>

namespace RayMan {

  static UnitVector3 GetFaceNormal(const UnitVector3& outwardNormal,
                                   const UnitVector3& rayDirection) {
    if (Dot(outwardNormal, rayDirection) < 0) {
      return outwardNormal;
    } else {
      return -outwardNormal;
    }
  }

  Hit::Hit(const Point3& point, const UnitVector3& outwardNormal, const Ray& ray,
           const Material* material)
      : point(point),
        normal(GetFaceNormal(outwardNormal, ray.GetDirection())),
        material(material) {}
}  // namespace RayMan