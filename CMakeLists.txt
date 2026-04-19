cmake_minimum_required(VERSION 3.10)

# Ensure option() honors parent-provided normal variables in subprojects.
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

project(xider
    VERSION 1.0.0
    DESCRIPTION "XIDER is an IDE for creating Desktop and XR applications with a focus on ease of use and rapid development."
    HOMEPAGE_URL "https://etib-corp.github.io/xider"
    LANGUAGES CXX
)

include(cmake/ProjectToolkit.cmake)

# Include common functions
include(cmake/CommonFunctions.cmake)

# Optional components
option(BUILD_XIDER_TESTING "Build the tests" OFF)
option(BUILD_XIDER_DOCS "Build documentation" OFF)
option(BUILD_XIDER_PLATFORMS "Build the platform-specific projects" OFF)

# Backend selection propagated to dependencies (notably evan)
set(_xider_default_glfw OFF)
set(_xider_default_openxr ON)

if(ANDROID)
    set(_xider_default_glfw OFF)
else()
    set(_xider_default_glfw ON)
endif()

option(XIDER_ENABLE_BACKEND_GLFW "Enable GLFW backend in evan" ${_xider_default_glfw})
option(XIDER_ENABLE_BACKEND_OPENXR "Enable OpenXR backend in evan" ${_xider_default_openxr})

set(BUILD_FOR_GLFW ${XIDER_ENABLE_BACKEND_GLFW} CACHE BOOL "Build evan with GLFW support" FORCE)
set(BUILD_FOR_OPENXR ${XIDER_ENABLE_BACKEND_OPENXR} CACHE BOOL "Build evan with OpenXR support" FORCE)

# Platform-specific configuration BEFORE fetching dependencies
if(ANDROID)
    include(cmake/ConfigAndroid.cmake)
elseif(WIN32)
    include(cmake/ConfigWindows.cmake)
elseif(APPLE)
    include(cmake/ConfigMacOS.cmake)
elseif(UNIX)
    include(cmake/ConfigLinux.cmake)
endif()

# Add the libraries subdirectory first to ensure dependencies are available for platform-specific targets
add_subdirectory(libraries)

add_subdirectory(platforms)

# Set global C++ standard and build defaults
xider_apply_project_defaults()

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

# Set common target properties
set_xider_target_properties(${PROJECT_NAME})

# Link private dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utility
        evan
        guillaume
)

# Configure public interface
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)

if(BUILD_XIDER_TESTING)
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

if(BUILD_XIDER_DOCS)
    file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h")

    xider_add_doxygen_docs(
        xider_docs
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
    message(STATUS "Skipping docs...")
endif()
