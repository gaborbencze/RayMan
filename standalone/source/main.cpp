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
#include <random>
#include <thread>

static constexpr int MaxColorValue = 255;
static constexpr int MaxReflectionDepth = 30;

static constexpr double Pi = 3.14159265358979323846;

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

static double GetRandomDouble(double min, double max) {
  static thread_local std::mt19937 generator;
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(generator);
}

static RayMan::UnitVector3 GetRandomUnitVector() {
  const auto a = GetRandomDouble(0, 2 * Pi);
  const auto z = GetRandomDouble(-1, 1);
  const auto r = std::sqrt(1 - z * z);
  return RayMan::UnitVector3(r * std::cos(a), r * std::sin(a), z);
}

static RayMan::Color RayColor(const RayMan::Ray& ray, const RayMan::Scene& world, int depth) {
  if (depth == 0) {
    return RayMan::Color::Black();
  }

  if (const auto hit = world.GetHit(ray)) {
    const auto reflectionRayDirection
        = RayMan::UnitVector3(hit->normal.ToVector3() + GetRandomUnitVector().ToVector3());
    const auto c = RayColor(RayMan::Ray(hit->point, reflectionRayDirection), world, depth - 1);
    return RayMan::Interpolate(RayMan::Color::Black(), c, 0.5);
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
  double sum_r = 0;
  double sum_g = 0;
  double sum_b = 0;

  for (int i = 0; i < samples; ++i) {
    const auto u = static_cast<double>(col + GetRandomDouble(0, 1)) / image.GetWidth();
    const auto v = 1 - (static_cast<double>(row + GetRandomDouble(0, 1)) / image.GetHeight());
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

static RayMan::Image RenderImage(int width, int height, int samples) {
  RayMan::Image img(height, width);
  const RayMan::Camera camera;

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
    ("o,out", "The output file name", cxxopts::value<std::string>())
    ("help", "Print usage");
  // clang-format on
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
  } else {
    const auto imageWidth = result["width"].as<int>();
    const auto imageHeight = result["height"].as<int>();
    const auto samplesPerPixel = result["samples"].as<int>();
    const auto outFileName = result["out"].as<std::string>();
    std::ofstream os(outFileName);

    PrintPPMImage(os, RenderImage(imageWidth, imageHeight, samplesPerPixel));
  }
}