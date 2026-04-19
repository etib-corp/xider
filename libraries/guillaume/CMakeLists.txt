cmake_minimum_required(VERSION 3.10)

project(guillaume
    VERSION 1.0.0
    DESCRIPTION "A C++ UI framework designed for modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/guillaume"
    LANGUAGES CXX
)

# Fetch external dependencies
include(FetchContent)

set(_xider_toolkit "${CMAKE_SOURCE_DIR}/cmake/ProjectToolkit.cmake")
if(EXISTS "${_xider_toolkit}")
    include("${_xider_toolkit}")
endif()

if(NOT TARGET utility)
    message(FATAL_ERROR "Target 'utility' is required. Add libraries/utility before libraries/guillaume in the monorepo root CMakeLists.txt.")
endif()

FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY https://github.com/sjinks/sqlite3-cmake
    GIT_TAG v3.49.1
)
FetchContent_MakeAvailable(sqlite3)

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
set_guillaume_target_properties(${PROJECT_NAME})

# Link private dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utility
        SQLite::SQLite3
)

# Configure public interface
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Optional components
option(BUILD_GUILLAUME_TESTING "Build the tests" OFF)
option(BUILD_GUILLAUME_DOCS "Build documentation" OFF)
option(BUILD_GUILLAUME_EXAMPLES "Build the examples" OFF)

if(BUILD_GUILLAUME_TESTING)
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

if(BUILD_GUILLAUME_DOCS)
    file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp")
    file(GLOB_RECURSE DOCS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.md")
    file(GLOB_RECURSE EXAMPLE_DOCS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.md")

    if(COMMAND xider_add_doxygen_docs)
        xider_add_doxygen_docs(
            guillaume_docs
            MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
            PROJECT_NAME "${PROJECT_NAME}"
            PROJECT_BRIEF "${PROJECT_DESCRIPTION}"
            PROJECT_NUMBER "${PROJECT_VERSION}"
            INPUTS
                ${HEADER_FILES}
                ${DOCS_FILES}
                ${EXAMPLE_DOCS_FILES}
                "${CMAKE_CURRENT_SOURCE_DIR}/CONTRIBUTING.md"
                "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
            COMMENT "Generating API documentation with Doxygen"
        )
    else()
        message(FATAL_ERROR "Shared Doxygen helper not found. Ensure cmake/ProjectToolkit.cmake is available from the monorepo root.")
    endif()
else()
    message(STATUS "Skipping docs...")
endif()
