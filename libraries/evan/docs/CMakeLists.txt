
include(${CMAKE_SOURCE_DIR}/cmake/Docs.cmake)

set(LIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
get_filename_component(LIB_ROOT_ABS "${LIB_ROOT}" ABSOLUTE)

add_doxygen_docs_for_library(evan "${LIB_ROOT_ABS}")
