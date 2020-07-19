#pragma once

#include <vector>

#include "Color.hpp"

namespace RayMan {
  class Image {
  public:
    Image(int height, int width);

    Color Get(int row, int col) const;
    void Set(int row, int col, const Color& c);

    int GetHeight() const;
    int GetWidth() const;

  private:
    const int height;
    const int width;

    std::vector<std::vector<Color>> pixels;
  };
}  // namespace RayMan