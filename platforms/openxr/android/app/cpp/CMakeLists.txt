cmake_minimum_required(VERSION 3.22.1)

project("xider_android")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 01. Policy & standard setup


# 02. Options & configuration


# 03. Global properties
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_compile_definitions(__OPENXR__)
add_compile_definitions(__ANDROID__)

add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(native_app_glue  PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
)

# Some third-party CMake checks rely on TRY_RUN to probe GNU strerror_r.
# Android builds are cross-compiled, so provide deterministic cache values.
if(ANDROID AND CMAKE_CROSSCOMPILING)
    # On Android/Bionic, GNU strerror_r is not available.
    set(HAVE_GNU_STRERROR_R_EXITCODE "1" CACHE STRING "Result for cross-compiled GNU strerror_r probe" FORCE)
    set(HAVE_GNU_STRERROR_R_EXITCODE__TRYRUN_OUTPUT "" CACHE STRING "Output for cross-compiled GNU strerror_r probe" FORCE)
endif()

# This CMake project is configured directly by Gradle (not by the workspace root CMake).
# Propagate backend/platform options so subprojects (notably evan) fetch/link OpenXR SDK.
set(BUILD_FOR_OPENXR ON CACHE BOOL "Build with OpenXR support" FORCE)
set(BUILD_FOR_ANDROID ON CACHE BOOL "Build for Android" FORCE)
set(BUILD_FOR_GLFW OFF CACHE BOOL "Build with GLFW support" FORCE)
set(BUILD_FOR_LINUX OFF CACHE BOOL "Build for Linux" FORCE)
set(BUILD_FOR_WINDOWS OFF CACHE BOOL "Build for Windows" FORCE)
set(BUILD_FOR_MACOS OFF CACHE BOOL "Build for macOS" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Build the tests" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "Build the documentation" FORCE)


if(NOT TARGET xider)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../libraries/xider ${CMAKE_CURRENT_BINARY_DIR}/xider)
endif()

# 04. Library definition
add_library(${PROJECT_NAME} SHARED
    main.cpp
)

# 06. Compile definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_VULKAN
    XR_USE_GRAPHICS_API_VULKAN_KHR
    VK_USE_PLATFORM_ANDROID_KHR
)

# 07. Link dependencies
target_link_libraries(${PROJECT_NAME}
    native_app_glue
    xider
    android
    log
)