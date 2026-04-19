cmake_minimum_required(VERSION 3.10)

project(utility
    VERSION 1.0.0
    DESCRIPTION "A comprehensive C++ utilities library tailored for the development of modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/utility"
    LANGUAGES CXX
)

# Include useful CMake modules
include(FetchContent)

set(_xider_toolkit "${CMAKE_SOURCE_DIR}/cmake/ProjectToolkit.cmake")
if(EXISTS "${_xider_toolkit}")
    include("${_xider_toolkit}")
endif()

# Include common functions
include(${CMAKE_SOURCE_DIR}/cmake/CommonFunctions.cmake)

# Platform-specific configuration
if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/ConfigWindows.cmake)
elseif(APPLE)
    include(${CMAKE_SOURCE_DIR}/cmake/ConfigMacOS.cmake)
elseif(UNIX)
    include(${CMAKE_SOURCE_DIR}/cmake/ConfigLinux.cmake)
endif()

# Set global C++ standard and build defaults
if(COMMAND xider_apply_project_defaults)
    xider_apply_project_defaults()
else()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()
endif()

# Fetch and configure dependencies
FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_BRANCH release
    GIT_TAG 966edceaf8cdca7996c4e9a1c5ced2938de63366
)
# tinyobjloader may call add_sanitizers() even when the helper module is unavailable.
if(NOT COMMAND add_sanitizers)
    function(add_sanitizers)
    endfunction()
endif()
FetchContent_MakeAvailable(tinyobjloader)

set(GLM_BUILD_TESTS OFF)

add_compile_definitions(GLM_FORCE_RADIANS)
add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO_ONE)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glm)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-0
)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(stb)

# Set the files directories
set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SOURCES_DIR}/*.cpp"
)

# Create the main library
add_library(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        GLM_ENABLE_EXPERIMENTAL
)

# Link common dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        tinyobjloader
        glm
        freetype
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            Dbghelp
    )
endif()

# Set common target properties
set_utility_target_properties(${PROJECT_NAME})

# Configure public interface
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${stb_SOURCE_DIR}>
)

# Optional components
option(BUILD_UTILITY_TESTING "Build the tests" OFF)
option(BUILD_UTILITY_DOCS "Build documentation" OFF)

if(BUILD_UTILITY_TESTING)
    include(CTest)
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

if(BUILD_UTILITY_DOCS)
    file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h")

    if(COMMAND xider_add_doxygen_docs)
        xider_add_doxygen_docs(
            utility_docs
            MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
            PROJECT_NAME "${PROJECT_NAME}"
            PROJECT_BRIEF "${PROJECT_DESCRIPTION}"
            PROJECT_NUMBER "${PROJECT_VERSION}"
            INPUTS
                ${HEADER_FILES}
                "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
            COMMENT "Generating API documentation with Doxygen"
        )
    else()
        message(FATAL_ERROR "Shared Doxygen helper not found. Ensure cmake/ProjectToolkit.cmake is available from the monorepo root.")
    endif()
else()
    message(STATUS "Skipping docs...")
endif()

if(BUILD_GRAPHICAL_TESTS)
    add_subdirectory(graphical_tests)
else()
    message(STATUS "Skipping graphical tests...")
endif()
