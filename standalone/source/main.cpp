#include <Image.hpp>
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

static RayMan::Image CreateSampleImage(int width, int height) {
  RayMan::Image img(height, width);
  for (int row = 0; row < img.GetHeight(); ++row) {
    for (int col = 0; col < img.GetWidth(); ++col) {
      img.Set(row, col,
              RayMan::Color(static_cast<double>(col) / (width - 1),
                            static_cast<double>(row) / (height - 1), 0.25));
    }
  }
  return img;
}

int main() { PrintPPMImage(std::cout, CreateSampleImage(1920, 1080)); }
