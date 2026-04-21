if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Skipping macOS desktop target on non-macOS host")
    return()
endif()

# Add subdirectory for local dependencies
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/xider ${CMAKE_CURRENT_BINARY_DIR}/xider)

# Set the name of the target
set(MACOS_DESKTOP_TARGET "xider-desktop-macos")

# Set the files directories
set(MACOS_DESKTOP_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(MACOS_DESKTOP_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${MACOS_DESKTOP_HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE MACOS_DESKTOP_SOURCES CONFIGURE_DEPENDS
    "${MACOS_DESKTOP_SOURCES_DIR}/*.cpp"
)

# Create the macOS desktop target
add_executable(${MACOS_DESKTOP_TARGET} ${MACOS_DESKTOP_SOURCES})

# Link with xider library
target_link_libraries(${MACOS_DESKTOP_TARGET} PRIVATE xider)
