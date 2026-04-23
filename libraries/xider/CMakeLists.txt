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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 01. Policy & standard setup

# 02. Options & configuration

# Fetch external dependencies

# Add subdirectory for local dependencies
if(NOT TARGET utility)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../utility ${CMAKE_CURRENT_BINARY_DIR}/utility)
endif()

if(NOT TARGET evan)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../evan ${CMAKE_CURRENT_BINARY_DIR}/evan)
endif()

if(NOT TARGET guillaume)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../guillaume ${CMAKE_CURRENT_BINARY_DIR}/guillaume)
endif()

# 03. Global properties

# Set the files directories
set(XIDER_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(XIDER_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

include_directories(
    ${XIDER_HEADERS_DIR}
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${XIDER_SOURCES_DIR}/*.cpp"
)

# 04. Library definition
add_library(${PROJECT_NAME} ${SOURCES})

# 07. Link dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utility
        evan
        guillaume
)

# 05. Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${XIDER_HEADERS_DIR}>
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