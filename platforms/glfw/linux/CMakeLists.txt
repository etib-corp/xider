if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Skipping Linux desktop target on non-Linux host")
    return()
endif()

# Add subdirectory for local dependencies
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/xider ${CMAKE_CURRENT_BINARY_DIR}/xider)

# Set the name of the target
set(LINUX_DESKTOP_TARGET "xider-desktop-linux")

# Set the files directories
set(LINUX_DESKTOP_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(LINUX_DESKTOP_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${LINUX_DESKTOP_HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE LINUX_DESKTOP_SOURCES CONFIGURE_DEPENDS
    "${LINUX_DESKTOP_SOURCES_DIR}/*.cpp"
)

# Create the Linux desktop target
add_executable(${LINUX_DESKTOP_TARGET} ${LINUX_DESKTOP_SOURCES})

target_link_libraries(${LINUX_DESKTOP_TARGET} PRIVATE xider)
