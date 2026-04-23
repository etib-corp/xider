cmake_minimum_required(VERSION 3.10)

project(utility
    VERSION 1.0.0
    DESCRIPTION "A comprehensive C++ utilities library tailored for the development of modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/utility"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 01. Policy & standard setup


# 02. Options & configuration


# 03. Global properties
include(FetchContent)


# Fetch external dependencies
FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v1.0.6
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

# Ensure FreeType is configured without HarfBuzz.
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable HarfBuzz in FreeType" FORCE)
set(FT_REQUIRE_HARFBUZZ OFF CACHE BOOL "Do not require HarfBuzz in FreeType" FORCE)

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

# 04. Library definition
add_library(${PROJECT_NAME} ${SOURCES})

# 06. Compile definitions
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        GLM_ENABLE_EXPERIMENTAL
)

# 07. Link dependencies
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

# 05. Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${stb_SOURCE_DIR}>
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