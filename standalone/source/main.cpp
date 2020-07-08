#include <Camera.hpp>
#include <Hit.hpp>
#include <Hittable/Sphere.hpp>
#include <Image.hpp>
#include <Ray.hpp>
#include <Scene.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <random>

static constexpr int MaxColorValue = 255;

static constexpr int ImageWidth = 960;
static constexpr int ImageHeight = 540;
static constexpr int SamplesPerPixel = 50;

static void PrintColor(std::ostream& os, const RayMan::Color& c) {
  const int r = c.r() * MaxColorValue;
  const int g = c.g() * MaxColorValue;
  const int b = c.b() * MaxColorValue;

  os << r << ' ' << g << ' ' << b << ' ';
}

static void PrintPPMImage(std::ostream& os, const RayMan::Image& img) {
  os << "P3\n";
  os << img.GetWidth() << ' ' << img.GetHeight() << '\n';
  os << MaxColorValue << '\n';

  for (int row = 0; row < img.GetHeight(); ++row) {
    for (int col = 0; col < img.GetWidth(); ++col) {
      const RayMan::Color c = img.Get(row, col);
      PrintColor(os, c);
    }
    os << '\n';
  }
}

static RayMan::Color RayColor(const RayMan::Ray& ray, const RayMan::Scene& world) {
  if (const auto hit = world.GetHit(ray)) {
    return RayMan::Color((hit->normal.x() + 1) * 0.5, (hit->normal.y() + 1) * 0.5,
                         (hit->normal.z() + 1) * 0.5);
  }

  const auto t = 0.5 * (ray.GetDirection().y() + 1);
  return RayMan::Interpolate(RayMan::Color(0.5, 0.7, 1), RayMan::Color(1, 1, 1), t);
}

static RayMan::Scene GetWorld() {
  RayMan::Scene world;

  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{0, 0, -1}, 0.5));
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{0, -100.5, 0}, 100));

  return world;
}

static RayMan::Color GetPixelColor(const RayMan::Image& image, const RayMan::Scene& world,
                                   const RayMan::Camera& camera, int samples, int row, int col) {
  // TODO: We should be able to call this from multiple threads
  // Making these static prevents this
  static std::uniform_real_distribution<double> distribution(0, 1);
  static std::mt19937 generator;

  double sum_r = 0;
  double sum_g = 0;
  double sum_b = 0;

  for (int i = 0; i < samples; ++i) {
    const auto u = static_cast<double>(col + distribution(generator)) / image.GetWidth();
    const auto v = 1 - (static_cast<double>(row + distribution(generator)) / image.GetHeight());
    const auto ray = camera.GetRay(u, v);
    const auto color = RayColor(ray, world);

    sum_r += color.r();
    sum_g += color.g();
    sum_b += color.b();
  }

  const auto scaleComponent = [samples](const double value) { return value / samples; };

  return RayMan::Color(scaleComponent(sum_r), scaleComponent(sum_g), scaleComponent(sum_b));
}

static RayMan::Image RenderImage(int width, int height, int samples) {
  RayMan::Image img(height, width);
  const RayMan::Camera camera;

  const RayMan::Scene world = GetWorld();

  for (int row = 0; row < img.GetHeight(); ++row) {
    for (int col = 0; col < img.GetWidth(); ++col) {
      img.Set(row, col, GetPixelColor(img, world, camera, samples, row, col));
    }
  }
  return img;
}

int main() { PrintPPMImage(std::cout, RenderImage(ImageWidth, ImageHeight, SamplesPerPixel)); }