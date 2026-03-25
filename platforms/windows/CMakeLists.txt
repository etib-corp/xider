if(NOT WIN32)
    message(STATUS "Skipping Windows desktop target on non-Windows host")
    return()
endif()

set(WINDOWS_DESKTOP_TARGET xider_windows_desktop)
set(WINDOWS_DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

add_xider_example(${WINDOWS_DESKTOP_TARGET} SOURCES ${WINDOWS_DESKTOP_SOURCES})
target_compile_definitions(${WINDOWS_DESKTOP_TARGET} PRIVATE XIDER_DESKTOP_PLATFORM_WINDOWS=1)
