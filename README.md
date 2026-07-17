# optimization
optimization research

## Build

Requires a C++23 compiler and, via Homebrew: `mlx`, `mlx-c`, `google-benchmark`.

    brew install mlx mlx-c google-benchmark
    make

This configures (on first run, or whenever the top-level `CMakeLists.txt` changes) and builds everything into `build/`. Run a benchmark, e.g.:

    ./build/matrix_multiplication

`make clean` removes the `build/` directory entirely.

If you'd rather drive CMake directly instead of the `Makefile` wrapper:

    cmake -B build
    cmake --build build

If CMake can't locate the Homebrew packages automatically, pass the prefix explicitly (only needs to be done once — the flag is cached in `build/CMakeCache.txt`, so a plain `make` afterwards reuses it):

    cmake -B build -DCMAKE_PREFIX_PATH="$(brew --prefix)"

Each subdirectory under `src/` is an independent experiment that builds its own executable. To add a new one, create `src/<name>/CMakeLists.txt` calling `add_experiment(<name> SOURCES <files...>)` — it's picked up automatically, no other files need editing.
