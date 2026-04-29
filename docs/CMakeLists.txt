
include(${CMAKE_SOURCE_DIR}/cmake/Docs.cmake)

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
get_filename_component(PROJECT_ROOT_ABS "${PROJECT_ROOT}" ABSOLUTE)

# Use a distinct name for the top-level project documentation
add_doxygen_docs_for_library(xider_project "${PROJECT_ROOT_ABS}")
