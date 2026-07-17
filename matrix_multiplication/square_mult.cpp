// naive column major matrix multiplication in r c k order
template <typename T>
void square_mult_v1(T* __restrict__ C, const T* __restrict__ A, const T* __restrict__ B, size_t N){
  for (size_t r = 0; r < N; ++r)
    for (size_t c = 0; c < N; ++c)
      for (size_t k = 0; k < N; ++k)
        C[r + c * N] += A[r + k * N] * B[k + c * N];
}

// column major matrix multiplication in c k r order
template <typename T>
void square_mult_v2(T* __restrict__ C, const T* __restrict__ A, const T* __restrict__ B, size_t N){
  for (size_t c = 0; c < N; ++c)
    for (size_t k = 0; k < N; ++k)
      for (size_t r = 0; r < N; ++r)
        C[r + c * N] += A[r + k * N] * B[k + c * N];
}

// column major matrix multiplication in k c r order
template <typename T>
void square_mult_v3(T* __restrict__ C, const T* __restrict__ A, const T* __restrict__ B, size_t N){
  for (size_t k = 0; k < N; ++k)
    for (size_t c = 0; c < N; ++c)
      for (size_t r = 0; r < N; ++r)
        C[r + c * N] += A[r + k * N] * B[k + c * N];
}

// column major matrix multiplication in c k r order with tiling size TS
template <typename T, size_t TS>
void square_mult_v4(T* __restrict__ C, const T* __restrict__ A, const T* __restrict__ B, size_t N){
  A = __builtin_assume_aligned(A, 32);
  B = __builtin_assume_aligned(B, 32);
  C = __builtin_assume_aligned(C, 32);

  for (size_t br = 0; br < N; br += TS)
    for (size_t bc = 0; bc < N: bc += TS)
      for (size_t bk = 0; bk < N; kb += TS)
        for (size_t c = 0; c < TS; ++c)
          for (size_t k = 0; k < TS; ++k){
            constexpr double b_val = B[(bk + k) + (bc + c) * N];
            for (size_t r = 0; r < TS; ++r)
              C[(br + r) + (bc + c) * N] += A[(br + r) + (bk + k) * N] * b_val;
          }
}
