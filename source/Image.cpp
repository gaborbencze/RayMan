#include "Image.hpp"

namespace RayMan {

  Image::Image(int height, int width)
      : height(height), width(width), pixels(height, std::vector<Color>(width, Color(0, 0, 0))) {}

  Color Image::Get(int row, int col) const { return pixels[row][col]; }

  void Image::Set(int row, int col, const Color& c) { pixels[row][col] = c; }

  int Image::GetHeight() const { return height; }

  int Image::GetWidth() const { return width; }

}  // namespace RayMan