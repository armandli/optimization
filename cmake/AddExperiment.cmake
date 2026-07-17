# add_experiment(<name>
#   SOURCES <src1> [<src2> ...]
#   [NO_BENCHMARK_MAIN]
# )
#
# Declares an executable target <name> for an experiment under src/. Links
# mlx, mlxc, and Google Benchmark uniformly so experiment CMakeLists.txt
# files stay minimal. By default the executable links benchmark::benchmark_main
# and gets a working main() for free; pass NO_BENCHMARK_MAIN if the
# experiment needs to supply its own main() (e.g. custom benchmark::Initialize
# handling), in which case it links benchmark::benchmark instead.
function(add_experiment name)
  set(options NO_BENCHMARK_MAIN)
  set(oneValueArgs)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT ARG_SOURCES)
    message(FATAL_ERROR "add_experiment(${name}): SOURCES must not be empty")
  endif()

  add_executable(${name} ${ARG_SOURCES})

  target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_features(${name} PRIVATE cxx_std_23)

  # mlx/mlxc already carry their own INTERFACE_INCLUDE_DIRECTORIES and
  # (for mlx) INTERFACE_COMPILE_OPTIONS via their imported-target
  # definitions, so plain linking is sufficient.
  target_link_libraries(${name} PRIVATE mlx mlxc)

  if(ARG_NO_BENCHMARK_MAIN)
    target_link_libraries(${name} PRIVATE benchmark::benchmark)
  else()
    target_link_libraries(${name} PRIVATE benchmark::benchmark_main)
  endif()
endfunction()
