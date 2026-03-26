# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("xider-android")

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

option(BUILD_FOR_OPENXR "Build for openxr" ON)

add_compile_definitions(__OPENXR__)
add_compile_definitions(__ANDROID__)

# Allow overriding tinyobjloader source when building in offline/corporate networks.
set(TINYOBJLOADER_GIT_TAG "v2.0.0rc13" CACHE STRING "tinyobjloader git tag")

# FetchContent uses the first declaration it sees for a dependency name.
# Declaring tinyobjloader here avoids bad declarations in transitive dependencies.
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

FetchContent_Declare(
        evan
        GIT_REPOSITORY https://github.com/etib-corp/evan.git
        GIT_TAG 48-feat-event-handling
        PATCH_COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/patches/evan.cmake"
)

FetchContent_MakeAvailable(evan)

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(${PROJECT_NAME} SHARED
        main.cpp
)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)

# Forces the linker to keep the JNI entry point for GameActivity
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

target_compile_definitions(${PROJECT_NAME} PUBLIC
        XR_USE_PLATFORM_ANDROID
        XR_USE_GRAPHICS_API_VULKAN
        XR_USE_GRAPHICS_API_VULKAN_KHR
        VK_USE_PLATFORM_ANDROID_KHR)

# Configure libraries CMake uses to link your target library.
target_link_libraries(${PROJECT_NAME}
        # The game activity
        game-activity::game-activity_static
        evan
        openxr_loader
        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log)
