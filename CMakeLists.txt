cmake_minimum_required(VERSION 3.25)

project(optimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Benchmarks are meaningless without optimizations enabled. Default to
# Release for single-config generators (Makefiles, Ninja) when the caller
# hasn't picked a build type explicitly. No effect on multi-config
# generators (Xcode), which ignore CMAKE_BUILD_TYPE.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# MLX, mlx-c and Google Benchmark are installed via Homebrew and resolve
# through plain find_package() as long as Homebrew's prefix is discoverable
# via CMake's default CMAKE_SYSTEM_PREFIX_PATH. If a machine doesn't pick
# these up automatically (e.g. an Intel Mac Homebrew prefix of /usr/local),
# pass the prefix explicitly instead of hardcoding a fallback path here:
#   cmake -B build -DCMAKE_PREFIX_PATH="$(brew --prefix)"
find_package(MLX REQUIRED)
find_package(MLXC REQUIRED)
find_package(benchmark REQUIRED)

include(AddExperiment)

# Place every target's final executable directly at the top of the build
# tree (build/<name>) instead of mirroring src/'s directory structure
# (build/src/<name>/<name>). Both the base variable (single-config
# generators, e.g. Makefiles/Ninja) and the per-config _<CONFIG> variants
# (multi-config generators, e.g. Xcode) must be set so output stays flat
# regardless of generator; CMAKE_CONFIGURATION_TYPES is empty for
# single-config generators, so the loop below is a no-op there.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
foreach(_config_type ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${_config_type} _config_type_upper)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_config_type_upper} ${CMAKE_BINARY_DIR})
endforeach()
unset(_config_type)
unset(_config_type_upper)

add_subdirectory(src)
