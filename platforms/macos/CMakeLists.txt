if(NOT APPLE)
    message(STATUS "Skipping macOS desktop target on non-macOS host")
    return()
endif()

set(MACOS_DESKTOP_TARGET xider_macos_desktop)
set(MACOS_DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

add_xider_example(${MACOS_DESKTOP_TARGET} SOURCES ${MACOS_DESKTOP_SOURCES})
target_compile_definitions(${MACOS_DESKTOP_TARGET} PRIVATE XIDER_DESKTOP_PLATFORM_MACOS=1)
