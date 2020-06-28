#include <Camera.hpp>
#include <Image.hpp>
#include <Ray.hpp>
#include <iostream>

static constexpr int MaxColorValue = 255;

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

static RayMan::Color RayColor(const RayMan::Ray& ray) {
  const auto t = 0.5 * (ray.GetDirection().y() + 1);
  return RayMan::Interpolate(RayMan::Color(0.5, 0.7, 1), RayMan::Color(1, 1, 1), t);
}

static RayMan::Image RenderImage(int width, int height) {
  RayMan::Image img(height, width);
  RayMan::Camera camera;

  for (int row = 0; row < img.GetHeight(); ++row) {
    for (int col = 0; col < img.GetWidth(); ++col) {
      const auto u = static_cast<double>(col) / (width - 1);
      const auto v = static_cast<double>(row) / (height - 1);
      const auto ray = camera.GetRay(u, v);
      img.Set(row, col, RayColor(ray));
    }
  }
  return img;
}

int main() { PrintPPMImage(std::cout, RenderImage(1920, 1080)); }
