#include <array>
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
#include <vector>

#include "Camera.hpp"
#include "Hit.hpp"
#include "Hittable/Sphere.hpp"
#include "Image.hpp"
#include "Material/Dielectric.hpp"
#include "Material/Lambertian.hpp"
#include "Material/Metal.hpp"
#include "RandomUtils.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

static constexpr int MaxColorValue = 255;
static constexpr int MaxReflectionDepth = 30;

static constexpr int ProgressBarWidth = 80;

static void PrintColor(std::ostream& os, const RayMan::Color& c) {
  const int r = static_cast<int>(c.r() * MaxColorValue);
  const int g = static_cast<int>(c.g() * MaxColorValue);
  const int b = static_cast<int>(c.b() * MaxColorValue);

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
    }
    return RayMan::Color::Black();
  }

  const auto t = 0.5 * (ray.GetDirection().y() + 1);

  const auto skyBottomColor = RayMan::Color(0.5, 0.7, 1);
  const auto skyTopColor = RayMan::Color(0.5, 0.7, 1);

  return RayMan::Interpolate(skyBottomColor, skyTopColor, t);
}
/*
[[maybe_unused]] static RayMan::Scene GetWorld() {
  RayMan::Scene world;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  const auto groundMaterial = std::make_shared<RayMan::Lambertian>(RayMan::Color(0.8, 0.8, 0));
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  const auto centerMaterial = std::make_shared<RayMan::Dielectric>(1.5);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  const auto leftMaterial = std::make_shared<RayMan::Metal>(RayMan::Color(0.8, 0.8, 0.8), 0.02);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  const auto rightMaterial = std::make_shared<RayMan::Metal>(RayMan::Color(0.1, 1, 0.5), 0.8);

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{0, -1000.5, 0}, 1000, groundMaterial));
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{0, 0, 0}, 0.5, centerMaterial));
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{-1, 0, 0}, 0.5, leftMaterial));
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  world.Add(std::make_unique<RayMan::Sphere>(RayMan::Point3{1, 0, 0}, 0.5, rightMaterial));

  return world;
}
*/
[[maybe_unused]] static RayMan::Scene GetRandomWorld() {
  std::vector<std::unique_ptr<RayMan::Hittable>> objects{};

  // ground
  {
    constexpr RayMan::Color groundColor(0.5, 0.5, 0.5);
    constexpr auto groundRadius = 10'000;
    const auto groundMaterial = std::make_shared<RayMan::Lambertian>(groundColor);
    objects.push_back(std::make_unique<RayMan::Sphere>(RayMan::Point3(0, -groundRadius, 0),
                                                       groundRadius, groundMaterial));
  }

  constexpr auto mainSpereRadius = 1;
  constexpr auto offset = 4;

  const auto mainSpheres
      = std::array<std::pair<RayMan::Point3, std::shared_ptr<RayMan::Material>>, 3>{
          std::pair(RayMan::Point3{offset, mainSpereRadius, 0},
                    std::make_shared<RayMan::Dielectric>(1.5)),
          std::pair(RayMan::Point3{-offset, mainSpereRadius, 0},
                    std::make_shared<RayMan::Lambertian>(RayMan::Color(0.2, 0.2, 0.5))),
          std::pair(RayMan::Point3{0, mainSpereRadius, 0},
                    std::make_shared<RayMan::Metal>(RayMan::Color(0.85, 0.85, 0.85), 0.1))};

  for (const auto& sphere : mainSpheres) {
    objects.push_back(
        std::make_unique<RayMan::Sphere>(sphere.first, mainSpereRadius, sphere.second));
  }

  // randomized small spheres
  {
    constexpr auto sceneSize = 11;
    constexpr auto maxOffset = 0.3;
    constexpr auto minRadius = 0.1;
    constexpr auto maxRadius = 0.3;

    constexpr auto GetRandomMaterial = []() -> std::shared_ptr<RayMan::Material> {
      constexpr auto GetRandomColor = [] {
        return RayMan::Color(RayMan::GetRandomDouble(0, 1), RayMan::GetRandomDouble(0, 1),
                             RayMan::GetRandomDouble(0, 1));
      };

      const auto rnd = RayMan::GetRandomDouble(0, 1);

      constexpr auto lamberdianProb = 0.8;
      constexpr auto metalProb = 0.15 + lamberdianProb;

      if (rnd < lamberdianProb) {
        const auto albedo = GetRandomColor();
        return std::make_shared<RayMan::Lambertian>(albedo);
      }
      if (rnd < metalProb) {
        const auto albedo = GetRandomColor();
        const auto fuzziness = RayMan::GetRandomDouble(0, 0.5);
        return std::make_shared<RayMan::Metal>(albedo, fuzziness);
      }
      static const auto glassMaterial = std::make_shared<RayMan::Dielectric>(1.5);
      return glassMaterial;
    };

    for (int x = -sceneSize; x <= sceneSize; ++x) {
      for (int z = -sceneSize; z <= sceneSize; ++z) {
        const auto IsCloseToMainSphere = [&mainSpheres](double x, double z) -> bool {
          return std::find_if(
                     begin(mainSpheres), end(mainSpheres),
                     [x, z](const auto& sphereInfo) {
                       const auto& [center, material] = sphereInfo;
                       return (center - RayMan::Point3(x, mainSpereRadius, z)).length_square() < 1;
                     })
                 != end(mainSpheres);
        };

        const auto radius = RayMan::GetRandomDouble(minRadius, maxRadius);
        const RayMan::Point3 center{x + RayMan::GetRandomDouble(-maxOffset, maxOffset), radius,
                                    z + RayMan::GetRandomDouble(-maxOffset, maxOffset)};

        if (IsCloseToMainSphere(center.x(), center.z())) {
          continue;
        }

        objects.push_back(std::make_unique<RayMan::Sphere>(center, radius, GetRandomMaterial()));
      }
    }
  }

  return RayMan::Scene{std::move(objects)};
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
  constexpr RayMan::Point3 cameraPosition{13, 2, 3};
  constexpr RayMan::Point3 cameraTarget{0, 0, 0};
  const auto camera = RayMan::Camera::Create(cameraPosition, cameraTarget, fov,
                                             static_cast<double>(width) / height, 0.1,
                                             (cameraTarget - cameraPosition).length());

  const RayMan::Scene world = GetRandomWorld();

  std::atomic<int> columnCounter(0);
  std::vector<std::thread> workers(std::thread::hardware_concurrency());
  for (auto& worker : workers) {
    worker = std::thread(RenderImageWorker, std::ref(camera), std::ref(world), std::ref(img),
                         samples, std::ref(columnCounter));
  }

  using namespace indicators;
  show_console_cursor(false);
  BlockProgressBar progressBar{option::BarWidth{ProgressBarWidth},
                               option::Start{"["},
                               option::End{"]"},
                               option::ForegroundColor{Color::white},
                               option::ShowElapsedTime{true},
                               option::ShowRemainingTime{true},
                               option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
                               option::MaxProgress{width}};

  while (columnCounter < width) {
    progressBar.set_progress(static_cast<float>(columnCounter));

    using namespace std::chrono_literals;
    constexpr auto updateDeltaTime = 100ms;
    std::this_thread::sleep_for(updateDeltaTime);
  }
  show_console_cursor(true);

  for (auto& thread : workers) {
    thread.join();
  }

  return img;
}

int main(int argc, char* argv[]) {
  try {
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

    if (result.count("help") > 0U) {
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
  } catch (const std::exception& ex) {
    std::cerr << "Exception: " << ex.what() << std::endl;
  } catch (...) {
    std::cerr << "An unexpected error occurred..." << std::endl;
  }
}