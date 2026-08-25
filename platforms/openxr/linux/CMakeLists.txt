if(NOT BUILD_FOR_LINUX)
    message(STATUS "Skipping Linux XR target because BUILD_FOR_LINUX is OFF")
    return()
endif()

# Set the name of the target
set(LINUX_XR_TARGET "xider-xr-linux")

# Set the files directories
set(LINUX_XR_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(LINUX_XR_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${LINUX_XR_HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE LINUX_XR_SOURCES CONFIGURE_DEPENDS
    "${LINUX_XR_SOURCES_DIR}/*.cpp"
)

# Create the Linux XR target
add_executable(${LINUX_XR_TARGET} ${LINUX_XR_SOURCES})

# Link with xider library
target_link_libraries(${LINUX_XR_TARGET} PRIVATE xider)


target_compile_definitions(${LINUX_XR_TARGET} PRIVATE XIDER_XR_PLATFORM_LINUX=1)
