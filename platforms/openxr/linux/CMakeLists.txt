if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Skipping Linux XR target on non-Linux host")
    return()
endif()

if(NOT TARGET utility)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/utility ${CMAKE_CURRENT_BINARY_DIR}/libraries/utility)
endif()

if(NOT TARGET evan)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/evan ${CMAKE_CURRENT_BINARY_DIR}/libraries/evan)
endif()

if(NOT TARGET guillaume)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/guillaume ${CMAKE_CURRENT_BINARY_DIR}/libraries/guillaume)
endif()

if(NOT TARGET xider)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../libraries/xider ${CMAKE_CURRENT_BINARY_DIR}/libraries/xider)
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
