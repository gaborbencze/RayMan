#include <Camera.hpp>
#include <Hit.hpp>
#include <Hittable/Sphere.hpp>
#include <Image.hpp>
#include <Ray.hpp>
#include <Scene.hpp>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cxxopts.hpp>
#include <fstream>
#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>

#include "Material/Lambertian.hpp"
#include "Material/Metal.hpp"
#include "RandomUtils.hpp"

static constexpr int MaxColorValue = 255;
static constexpr int MaxReflectionDepth = 30;

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

static RayMan::Color RayColor(const RayMan::Ray& ray, const RayMan::Scene& world, int depth) {
  if (depth == 0) {
    return RayMan::Color::Black();
  }

  if (const auto hit = world.GetHit(ray)) {
    if (const auto scatter = hit->material->Scatter(ray, *hit)) {
      const auto [attenuation, scatteredRay] = *scatter;
      return attenuation * RayColor(scatteredRay, world, depth - 1);
    } else {
      return RayMan::Color::Black();
    }
  }

  const auto t = 0.5 * (ray.GetDirection().y() + 1);
  return RayMan::Interpolate(RayMan::Color(0.5, 0.7, 1), RayMan::Color(1, 1, 1), t);
}

static RayMan::Scene GetWorld() {
  RayMan::Scene world;

  const auto groundMaterial = std::make_shared<RayMan::Lambertian>(RayMan::Color(0.8, 0.8, 0));
  const auto centerMaterial = std::make_shared<RayMan::Lambertian>(RayMan::Color(0.7, 0.3, 0.3));
  const auto leftMaterial = std::make_shared<RayMan::Metal>(RayMan::Color(0.8, 0.8, 0.8), 0.02);
  const auto rightMaterial = std::make_shared<RayMan::Metal>(RayMan::Color(0.8, 0.6, 0.2), 0.8);

  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{0, -1000.5, 0}, 1000, groundMaterial));
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{0, 0, 0}, 0.5, centerMaterial));
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{-1, 0, 0}, 0.5, leftMaterial));
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{1, 0, 0}, 0.5, rightMaterial));

  return world;
}

static RayMan::Color GetPixelColor(const RayMan::Image& image, const RayMan::Scene& world,
                                   const RayMan::Camera& camera, int samples, int row, int col) {
  double sum_r = 0;
  double sum_g = 0;
  double sum_b = 0;

  for (int i = 0; i < samples; ++i) {
    const auto u = static_cast<double>(col + RayMan::GetRandomDouble(0, 1)) / image.GetWidth();
    const auto v
        = 1 - (static_cast<double>(row + RayMan::GetRandomDouble(0, 1)) / image.GetHeight());
    const auto ray = camera.GetRay(u, v);
    const auto color = RayColor(ray, world, MaxReflectionDepth);

    sum_r += color.r();
    sum_g += color.g();
    sum_b += color.b();
  }

  const auto scaleComponent = [samples](const double value) { return std::sqrt(value / samples); };

  return RayMan::Color(scaleComponent(sum_r), scaleComponent(sum_g), scaleComponent(sum_b));
}

static void RenderImageColumn(const RayMan::Camera& camera, const RayMan::Scene& world,
                              RayMan::Image& image, int samples, int col) {
  for (int row = 0; row < image.GetHeight(); ++row) {
    image.Set(row, col, GetPixelColor(image, world, camera, samples, row, col));
  }
}

static void RenderImageWorker(const RayMan::Camera& camera, const RayMan::Scene& world,
                              RayMan::Image& image, int samples, std::atomic<int>& columnCounter) {
  for (int col = columnCounter++; col < image.GetWidth(); col = columnCounter++) {
    RenderImageColumn(camera, world, image, samples, col);
  }
}

static RayMan::Image RenderImage(int width, int height, double fov, int samples) {
  RayMan::Image img(height, width);
  const auto camera
      = RayMan::Camera::Create({2, -0.3, 3}, {0, 0, 0}, fov, static_cast<double>(width) / height);

  const RayMan::Scene world = GetWorld();

  std::atomic<int> columnCounter(0);
  std::vector<std::thread> workers;
  for (auto i = 0u; i < std::thread::hardware_concurrency(); ++i) {
    workers.emplace_back(RenderImageWorker, std::ref(camera), std::ref(world), std::ref(img),
                         samples, std::ref(columnCounter));
  }

  using namespace indicators;
  show_console_cursor(false);
  BlockProgressBar progressBar{option::BarWidth{80},
                               option::Start{"["},
                               option::End{"]"},
                               option::ForegroundColor{Color::white},
                               option::ShowElapsedTime{true},
                               option::ShowRemainingTime{true},
                               option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
                               option::MaxProgress{width}};

  while (columnCounter < width) {
    using namespace std::chrono_literals;
    progressBar.set_progress(columnCounter);
    std::this_thread::sleep_for(100ms);
  }
  show_console_cursor(true);

  for (auto& thread : workers) {
    thread.join();
  }

  return img;
}

int main(int argc, char* argv[]) {
  cxxopts::Options options("RayMan", "RayMan is a very basic raytracer");
  // clang-format off
  options.add_options()
    ("w,width", "Width of the output image", cxxopts::value<int>())
    ("h,height", "Height of the output image", cxxopts::value<int>())
    ("s,samples", "Number of samples per pixel", cxxopts::value<int>())
    ("f,fov", "Vertial field-of-view (degrees)", cxxopts::value<double>())
    ("o,out", "The output file name", cxxopts::value<std::string>())
    ("help", "Print usage");
  // clang-format on
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
  } else {
    const auto imageWidth = result["width"].as<int>();
    const auto imageHeight = result["height"].as<int>();
    const auto fov = result["fov"].as<double>();
    const auto samplesPerPixel = result["samples"].as<int>();
    const auto outFileName = result["out"].as<std::string>();
    std::ofstream os(outFileName);

    PrintPPMImage(os, RenderImage(imageWidth, imageHeight, fov, samplesPerPixel));
  }
}