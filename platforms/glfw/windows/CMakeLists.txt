if(NOT BUILD_FOR_WINDOWS)
    message(STATUS "Skipping Windows desktop target because BUILD_FOR_WINDOWS is OFF")
    return()
endif()

# Add subdirectory for local dependencies
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/xider ${CMAKE_CURRENT_BINARY_DIR}/xider)

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

