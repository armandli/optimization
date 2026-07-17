#include <cstddef>
#include <cstdlib>

#include <algorithm>

#include <benchmark/benchmark.h>

#include <square_mult.hpp>

namespace {

constexpr size_t kAlignment = 32;
constexpr size_t kTileSize = 64;

template <typename T>
T* AllocMatrix(size_t N) {
  size_t bytes = N * N * sizeof(T);
  size_t padded_bytes = ((bytes + kAlignment - 1) / kAlignment) * kAlignment;
  void* raw = std::aligned_alloc(kAlignment, padded_bytes);
  return static_cast<T*>(raw);
}

template <typename T>
void FreeMatrix(T* ptr) {
  std::free(ptr);
}

// Covers v1/v2/v3, which share a signature; Fn is fixed per instantiation.
template <typename T, void (*Fn)(T*, const T*, const T*, size_t)>
void BM_SquareMult(benchmark::State& state) {
  const size_t N = static_cast<size_t>(state.range(0));

  T* A = AllocMatrix<T>(N);
  T* B = AllocMatrix<T>(N);
  T* C = AllocMatrix<T>(N);
  std::fill(A, A + N * N, T{1});
  std::fill(B, B + N * N, T{1});
  std::fill(C, C + N * N, T{0});

  for (auto _ : state) {
    Fn(C, A, B, N);
    benchmark::DoNotOptimize(C);
  }

  state.SetComplexityN(static_cast<int64_t>(N));
  FreeMatrix(A);
  FreeMatrix(B);
  FreeMatrix(C);
}

// v4 additionally takes a compile-time tile size TS; N must be a multiple
// of TS (the kernel does no remainder handling).
template <typename T, size_t TS>
void BM_SquareMultV4(benchmark::State& state) {
  const size_t N = static_cast<size_t>(state.range(0));

  T* A = AllocMatrix<T>(N);
  T* B = AllocMatrix<T>(N);
  T* C = AllocMatrix<T>(N);
  std::fill(A, A + N * N, T{1});
  std::fill(B, B + N * N, T{1});
  std::fill(C, C + N * N, T{0});

  for (auto _ : state) {
    square_mult_v4<T, TS>(C, A, B, N);
    benchmark::DoNotOptimize(C);
  }

  state.SetComplexityN(static_cast<int64_t>(N));
  FreeMatrix(A);
  FreeMatrix(B);
  FreeMatrix(C);
}

}  // namespace

BENCHMARK(BM_SquareMult<double, square_mult_v1<double>>)
    ->RangeMultiplier(2)->Range(64, 512)
    ->Unit(benchmark::kMillisecond)->Complexity(benchmark::oNCubed);
BENCHMARK(BM_SquareMult<double, square_mult_v2<double>>)
    ->RangeMultiplier(2)->Range(64, 512)
    ->Unit(benchmark::kMillisecond)->Complexity(benchmark::oNCubed);
BENCHMARK(BM_SquareMult<double, square_mult_v3<double>>)
    ->RangeMultiplier(2)->Range(64, 512)
    ->Unit(benchmark::kMillisecond)->Complexity(benchmark::oNCubed);
BENCHMARK(BM_SquareMultV4<double, kTileSize>)
    ->RangeMultiplier(2)->Range(64, 512)
    ->Unit(benchmark::kMillisecond)->Complexity(benchmark::oNCubed);
