cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(RayManStandalone
  LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME cxxopts
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.0
  OPTIONS
    "CXXOPTS_BUILD_EXAMPLES Off"
    "CXXOPTS_BUILD_TESTS Off"
)

CPMAddPackage(
  NAME indicators
  GITHUB_REPOSITORY p-ranav/indicators
  VERSION 1.9
)

CPMAddPackage(
  NAME RayMan
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)

find_package(Threads REQUIRED)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(RayManStandalone ${sources})

set_target_properties(RayManStandalone PROPERTIES 
  CXX_STANDARD 17 
  OUTPUT_NAME "RayMan"
)

# set warning levels
if(MSVC)
  target_compile_options(RayManStandalone PRIVATE /W4 /WX)
else()
  target_compile_options(RayManStandalone PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(RayManStandalone PRIVATE Threads::Threads RayMan cxxopts indicators)
