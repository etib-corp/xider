if(NOT UNIX OR APPLE)
    message(STATUS "Skipping Linux desktop target on non-Linux host")
    return()
endif()

set(LINUX_DESKTOP_TARGET xider_linux_desktop)
set(LINUX_DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

add_xider_example(${LINUX_DESKTOP_TARGET} SOURCES ${LINUX_DESKTOP_SOURCES})
target_compile_definitions(${LINUX_DESKTOP_TARGET} PRIVATE XIDER_DESKTOP_PLATFORM_LINUX=1)
