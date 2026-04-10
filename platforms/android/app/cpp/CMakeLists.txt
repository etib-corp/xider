cmake_minimum_required(VERSION 3.22.1)

project("xider-android")

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

option(BUILD_FOR_OPENXR "Build for openxr" ON)

add_compile_definitions(__OPENXR__)
add_compile_definitions(__ANDROID__)

# -----------------------------
# NativeActivity (IMPORTANT)
# -----------------------------
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue  PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


# -----------------------------
# tinyobjloader
# -----------------------------
set(TINYOBJLOADER_GIT_TAG "v2.0.0rc13" CACHE STRING "tinyobjloader git tag")

if(DEFINED FETCHCONTENT_SOURCE_DIR_TINYOBJLOADER)
    FetchContent_Declare(
        tinyobjloader
        SOURCE_DIR ${FETCHCONTENT_SOURCE_DIR_TINYOBJLOADER}
    )
else()
    FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG ${TINYOBJLOADER_GIT_TAG}
        GIT_SHALLOW TRUE
    )
endif()

# -----------------------------
# evan
# -----------------------------
FetchContent_Declare(
    evan
    GIT_REPOSITORY https://github.com/etib-corp/evan.git
    GIT_TAG 48-feat-event-handling
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(evan)

# -----------------------------
# Main lib
# -----------------------------
add_library(${PROJECT_NAME} SHARED
    main.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_VULKAN
    XR_USE_GRAPHICS_API_VULKAN_KHR
    VK_USE_PLATFORM_ANDROID_KHR
)

# -----------------------------
# Link
# -----------------------------
target_link_libraries(${PROJECT_NAME}
    native_app_glue
    evan
    openxr_loader
    EGL
    GLESv3
    jnigraphics
    android
    log
)