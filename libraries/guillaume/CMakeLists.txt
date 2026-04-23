cmake_minimum_required(VERSION 3.10)

project(guillaume
    VERSION 1.0.0
    DESCRIPTION "A C++ UI framework designed for modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/guillaume"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 01. Policy & standard setup

# 02. Options & configuration

# 03. Global properties

# Fetch external dependencies
include(FetchContent)

FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY https://github.com/sjinks/sqlite3-cmake
    GIT_TAG v3.49.1
)
FetchContent_MakeAvailable(sqlite3)

# Add subdirectory for local dependencies
if(NOT TARGET utility)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../utility ${CMAKE_CURRENT_BINARY_DIR}/utility)
endif()

# 04. Library definition

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


add_library(${PROJECT_NAME} ${SOURCES})

# 07. Link dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utility
        SQLite::SQLite3
)

# 05. Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)

# 08. Documentations

if(BUILD_DOCS)
    message(STATUS "Building documentation...")
else()
    message(STATUS "Skipping documentation...")
endif()

# 09. Tests

if(BUILD_TESTING)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()