cmake_minimum_required(VERSION 3.10)

project(xider
    VERSION 1.0.0
    DESCRIPTION "XIDER is an advanced IDE (Integrated Development Environment) designed to develop XR (eXtended Reality) applications. It provides a comprehensive set of tools and features to streamline the development process for VR (Virtual Reality), AR (Augmented Reality), and MR (Mixed Reality) projects."
    HOMEPAGE_URL "https://etib-corp.github.io/xider"
    LANGUAGES CXX
)

# Include common functions
include(FetchContent)
include(cmake/CommonFunctions.cmake)

# Platform-specific configuration
if(WIN32)
    include(cmake/ConfigWindows.cmake)
elseif(APPLE)
    include(cmake/ConfigMacOS.cmake)
elseif(UNIX)
    include(cmake/ConfigLinux.cmake)
endif()

# Set global C++ standard and properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Backend selection
if(ANDROID)
    set(BUILD_FOR_OPENXR ON CACHE BOOL "Enable OpenXR backend" FORCE)
else()
    set(BUILD_FOR_GLFW ON CACHE BOOL "Enable GLFW backend" FORCE)
endif()

# Also add compile definitions for our targets
if(BUILD_FOR_OPENXR)
    add_compile_definitions(BUILD_FOR_OPENXR)
endif()
if(BUILD_FOR_GLFW)
    add_compile_definitions(BUILD_FOR_GLFW)
endif()


FetchContent_Declare(
    evan
    GIT_REPOSITORY https://github.com/etib-corp/evan.git
    GIT_TAG main
)
FetchContent_MakeAvailable(evan)

FetchContent_Declare(
    guillaume
    GIT_REPOSITORY https://github.com/etib-corp/guillaume.git
    GIT_TAG main
)
FetchContent_MakeAvailable(guillaume)

# Set the files directories
set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE SOURCES
    "${SOURCES_DIR}/*.cpp"
)

# Create the main library
add_library(${PROJECT_NAME} ${SOURCES})

# Set common target properties
set_xider_target_properties(${PROJECT_NAME})

# Configure public interface
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Set library output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Optional components
option(BUILD_TESTING "Build the tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_EXAMPLES "Build the examples" OFF)

if(BUILD_TESTING)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

if(BUILD_DOCS)
    message(STATUS "Building docs...")
    add_subdirectory(docs)
else()
    message(STATUS "Skipping docs...")
endif()
