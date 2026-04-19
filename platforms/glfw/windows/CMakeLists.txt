if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Skipping Windows desktop target on non-Windows host")
    return()
endif()

# Set the name of the target
set(WINDOWS_DESKTOP_TARGET "xider-desktop-windows")

# Set the files directories
set(WINDOWS_DESKTOP_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(WINDOWS_DESKTOP_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${WINDOWS_DESKTOP_HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE WINDOWS_DESKTOP_SOURCES CONFIGURE_DEPENDS
    "${WINDOWS_DESKTOP_SOURCES_DIR}/*.cpp"
)

# Create the Windows desktop target
add_executable(${WINDOWS_DESKTOP_TARGET} ${WINDOWS_DESKTOP_SOURCES})

# Link with xider library
target_link_libraries(${WINDOWS_DESKTOP_TARGET} PRIVATE xider)

# Set common properties
set_target_properties(${WINDOWS_DESKTOP_TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_definitions(${WINDOWS_DESKTOP_TARGET} PRIVATE XIDER_DESKTOP_PLATFORM_WINDOWS=1)
