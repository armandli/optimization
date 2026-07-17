# Auto-discovers experiment subdirectories: any immediate child directory
# of src/ containing a CMakeLists.txt is added automatically. Adding a new
# experiment therefore requires no edits here — just create
# src/<new_experiment>/CMakeLists.txt.
#
# CONFIGURE_DEPENDS makes CMake re-glob (and thus notice new/removed
# experiment directories) whenever the configure step reruns.
file(GLOB _experiment_candidates
  LIST_DIRECTORIES true
  CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*
)

foreach(_candidate ${_experiment_candidates})
  if(IS_DIRECTORY ${_candidate} AND EXISTS ${_candidate}/CMakeLists.txt)
    add_subdirectory(${_candidate})
  endif()
endforeach()

unset(_experiment_candidates)
