cmake_minimum_required(VERSION 3.22.1)

project("xider-android")

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