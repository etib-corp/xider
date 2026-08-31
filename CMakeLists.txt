cmake_minimum_required(VERSION 3.10)

# Ensure option() honors parent-provided normal variables in subprojects.
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

project(xider
    VERSION 1.0.0
    DESCRIPTION "XIDER is an IDE for creating Desktop and XR applications with a focus on ease of use and rapid development."
    HOMEPAGE_URL "https://etib-corp.github.io/xider"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_FOR_OPENXR "Build with OpenXR support" OFF)
option(BUILD_FOR_GLFW "Build with GLFW support" OFF)
option(BUILD_FOR_ANDROID "Build for Android" OFF)
option(BUILD_FOR_LINUX "Build for Linux" OFF)
option(BUILD_FOR_WINDOWS "Build for Windows" OFF)
option(BUILD_FOR_MACOS "Build for macOS" OFF)

option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_TESTING "Build the tests" OFF)
option(BUILD_BENCHMARKS "Build the benchmark harness" OFF)
option(DEV_MODE "Enable development mode" OFF)

# Change default output of bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_EXECUTABLE_SUFFIX ".bin")

# Option to build documentation for all libraries
option(BUILD_ALL_DOCS "Build documentation for all libraries" OFF)

# Allow building docs without selecting a platform/backend
option(BUILD_DOCS_ONLY "Configure project for documentation only (skip platform/backend checks)" OFF)

set(_SKIP_PLATFORM_VALIDATION FALSE)
if(BUILD_ALL_DOCS OR BUILD_DOCS_ONLY)
    set(_SKIP_PLATFORM_VALIDATION TRUE)
endif()

set(PLATFORM_OPTION_COUNT 0)

if(BUILD_FOR_ANDROID)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()
if(BUILD_FOR_LINUX)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()
if(BUILD_FOR_WINDOWS)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()
if(BUILD_FOR_MACOS)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()

if(NOT _SKIP_PLATFORM_VALIDATION)
    if(NOT PLATFORM_OPTION_COUNT EQUAL 1)
        message(FATAL_ERROR "Invalid platform configuration: enable exactly one platform option (BUILD_FOR_ANDROID, BUILD_FOR_LINUX, BUILD_FOR_WINDOWS, BUILD_FOR_MACOS).")
    endif()
endif()

set(_backend_option_count 0)
if(BUILD_FOR_OPENXR)
    math(EXPR _backend_option_count "${_backend_option_count} + 1")
endif()
if(BUILD_FOR_GLFW)
    math(EXPR _backend_option_count "${_backend_option_count} + 1")
endif()

if(NOT _SKIP_PLATFORM_VALIDATION)
    if(NOT _backend_option_count EQUAL 1)
        message(FATAL_ERROR "Invalid backend configuration: enable exactly one backend option (BUILD_FOR_OPENXR or BUILD_FOR_GLFW).")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()

# Android builds are delegated to Gradle (NDK), which fetches its own copies
# of the sibling libraries. The host CMake must not build them.
set(XIDER_BUILD_HOST_LIBRARY TRUE)
if(BUILD_FOR_ANDROID)
    set(XIDER_BUILD_HOST_LIBRARY FALSE)
endif()

# 03. Global properties

# Fetch external dependencies (standalone sibling libraries)
if(NOT _SKIP_PLATFORM_VALIDATION AND XIDER_BUILD_HOST_LIBRARY)
    include(FetchContent)

    # Disable sibling-library tests while fetching; the siblings are consumed
    # here as libraries only (their tests run in their own CI).
    set(XIDER_BUILD_TESTING ${BUILD_TESTING})
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(utility
        GIT_REPOSITORY https://github.com/etib-corp/utility.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    # Prefer local sibling checkouts (../utility, ../evan, ../guillaume) for
    # development so that local fixes are picked up without pushing first.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../utility/CMakeLists.txt")
        set(FETCHCONTENT_SOURCE_DIR_UTILITY
            "${CMAKE_CURRENT_SOURCE_DIR}/../utility" CACHE PATH "" FORCE)
    endif()
    FetchContent_MakeAvailable(utility)

    FetchContent_Declare(evan
        GIT_REPOSITORY https://github.com/etib-corp/evan.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../evan/CMakeLists.txt")
        set(FETCHCONTENT_SOURCE_DIR_EVAN
            "${CMAKE_CURRENT_SOURCE_DIR}/../evan" CACHE PATH "" FORCE)
    endif()
    FetchContent_MakeAvailable(evan)

    FetchContent_Declare(guillaume
        GIT_REPOSITORY https://github.com/etib-corp/guillaume.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../guillaume/CMakeLists.txt")
        set(FETCHCONTENT_SOURCE_DIR_GUILLAUME
            "${CMAKE_CURRENT_SOURCE_DIR}/../guillaume" CACHE PATH "" FORCE)
    endif()
    FetchContent_MakeAvailable(guillaume)

    set(BUILD_TESTING ${XIDER_BUILD_TESTING} CACHE BOOL "" FORCE)
endif()

# 04. Library definition

if(XIDER_BUILD_HOST_LIBRARY)
    # Set the files directories
    set(XIDER_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")

    # Explicit source list (no file(GLOB)) for reproducible packaging.
    include(cmake/Sources.cmake)

    add_library(${PROJECT_NAME} ${XIDER_SOURCES})

    # 07. Link dependencies
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            utility
            evan
            guillaume
    )

    # 05. Include directories (target-scoped)
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${XIDER_HEADERS_DIR}>
            $<INSTALL_INTERFACE:include>
    )
endif()

# Platform-specific entry points
if(NOT _SKIP_PLATFORM_VALIDATION)
    if (BUILD_FOR_OPENXR)
        message(STATUS "Building with OpenXR support")

        if(BUILD_FOR_ANDROID)
            add_subdirectory(platforms/openxr/android)
        elseif(BUILD_FOR_WINDOWS)
            message(FATAL_ERROR "OpenXR support for Windows is not implemented yet.")
        elseif(BUILD_FOR_MACOS)
            message(FATAL_ERROR "OpenXR support for macOS is not implemented yet.")
        elseif(BUILD_FOR_LINUX)
            add_subdirectory(platforms/openxr/linux)
        else()
            message(FATAL_ERROR "Unsupported platform for OpenXR: enable one of BUILD_FOR_ANDROID, BUILD_FOR_LINUX, BUILD_FOR_WINDOWS, or BUILD_FOR_MACOS.")
        endif()
    elseif (BUILD_FOR_GLFW)
        message(STATUS "Building with GLFW support")

        if(BUILD_FOR_ANDROID)
            add_subdirectory(platforms/glfw/android)
        elseif(BUILD_FOR_LINUX)
            add_subdirectory(platforms/glfw/linux)
        elseif(BUILD_FOR_WINDOWS)
            add_subdirectory(platforms/glfw/windows)
        elseif(BUILD_FOR_MACOS)
            add_subdirectory(platforms/glfw/macos)
        else()
            message(FATAL_ERROR "Unsupported platform for GLFW: enable one of BUILD_FOR_ANDROID, BUILD_FOR_LINUX, BUILD_FOR_WINDOWS, or BUILD_FOR_MACOS.")
        endif()
    else()
        message(FATAL_ERROR "Invalid backend configuration.")
    endif()
else()
    message(STATUS "Configuring for documentation-only build; skipping platform/backend subdirectories.")
endif()

# If requested, include docs helper and add unified documentation
if(BUILD_ALL_DOCS OR BUILD_DOCS_ONLY)
    include(${CMAKE_SOURCE_DIR}/cmake/Docs.cmake)
    add_subdirectory(docs)
endif()

if (DEV_MODE AND XIDER_BUILD_HOST_LIBRARY)
    message(STATUS "Development mode enabled.")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEV)
endif()

# 09. Tests

if(BUILD_TESTING AND XIDER_BUILD_HOST_LIBRARY)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

# 10. Benchmarks

if(BUILD_BENCHMARKS AND XIDER_BUILD_HOST_LIBRARY)
    message(STATUS "Building benchmarks...")
    add_subdirectory(benchmarks)
endif()

# 11. Examples

if(XIDER_BUILD_HOST_LIBRARY)
    add_subdirectory(examples)
endif()
