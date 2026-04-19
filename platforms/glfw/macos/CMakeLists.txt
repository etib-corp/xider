if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Skipping macOS desktop target on non-macOS host")
    return()
endif()

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

# Set common properties
set_target_properties(${MACOS_DESKTOP_TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_definitions(${MACOS_DESKTOP_TARGET} PRIVATE XIDER_DESKTOP_PLATFORM_MACOS=1)
