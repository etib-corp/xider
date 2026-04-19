# Common functions for XIDER project

set(_xider_toolkit "${CMAKE_SOURCE_DIR}/cmake/ProjectToolkit.cmake")
if(EXISTS "${_xider_toolkit}")
    include("${_xider_toolkit}")
endif()

# Function to create an example executable
function(add_xider_platform EXAMPLE_NAME)
    # Parse optional arguments
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES" ${ARGN})

    # Set example directories
    set(EXAMPLE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
    set(EXAMPLE_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

    # Find source files if not provided
    if(NOT ARG_SOURCES)
        file(GLOB_RECURSE EXAMPLE_SOURCES "${EXAMPLE_SOURCES_DIR}/*.cpp")
    else()
        set(EXAMPLE_SOURCES ${ARG_SOURCES})
    endif()

    # Create executable
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})

    # Link with xider library
    target_link_libraries(${EXAMPLE_NAME} PRIVATE xider)

    # Link additional libraries if provided
    if(ARG_LIBRARIES)
        target_link_libraries(${EXAMPLE_NAME} PRIVATE ${ARG_LIBRARIES})
    endif()

    # Set common properties
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    # Include example headers if they exist
    if(EXISTS "${EXAMPLE_HEADERS_DIR}")
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${EXAMPLE_HEADERS_DIR})
    endif()

    message(STATUS "Added example: ${EXAMPLE_NAME}")
endfunction()

# Function to set common target properties
function(set_xider_target_properties TARGET_NAME)
    if(COMMAND xider_apply_target_defaults)
        xider_apply_target_defaults(${TARGET_NAME})
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )

        # Add platform-specific compile definitions
        if(WIN32)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_WINDOWS)
        elseif(APPLE)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_MACOS)
        elseif(UNIX)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_LINUX)
        endif()
    endif()
endfunction()

# Function to create an example executable for Evan
function(add_evan_example EXAMPLE_NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES" ${ARGN})

    set(EXAMPLE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
    set(EXAMPLE_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

    if(NOT ARG_SOURCES)
        file(GLOB_RECURSE EXAMPLE_SOURCES "${EXAMPLE_SOURCES_DIR}/*.cpp")
    else()
        set(EXAMPLE_SOURCES ${ARG_SOURCES})
    endif()

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE evan)

    if(ARG_LIBRARIES)
        target_link_libraries(${EXAMPLE_NAME} PRIVATE ${ARG_LIBRARIES})
    endif()

    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    if(EXISTS "${EXAMPLE_HEADERS_DIR}")
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${EXAMPLE_HEADERS_DIR})
    endif()

    message(STATUS "Added example: ${EXAMPLE_NAME}")
endfunction()

# Function to create an example executable for Guillaume
function(add_guillaume_example EXAMPLE_NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES" ${ARGN})

    set(EXAMPLE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
    set(EXAMPLE_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

    if(NOT ARG_SOURCES)
        file(GLOB_RECURSE EXAMPLE_SOURCES "${EXAMPLE_SOURCES_DIR}/*.cpp")
    else()
        set(EXAMPLE_SOURCES ${ARG_SOURCES})
    endif()

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE guillaume)

    if(ARG_LIBRARIES)
        target_link_libraries(${EXAMPLE_NAME} PRIVATE ${ARG_LIBRARIES})
    endif()

    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    if(EXISTS "${EXAMPLE_HEADERS_DIR}")
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${EXAMPLE_HEADERS_DIR})
    endif()

    message(STATUS "Added example: ${EXAMPLE_NAME}")
endfunction()

# Function to set common target properties for Utility
function(set_utility_target_properties TARGET_NAME)
    if(COMMAND xider_apply_target_defaults)
        xider_apply_target_defaults(${TARGET_NAME})
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )

        if(WIN32)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_WINDOWS)
        elseif(APPLE)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_MACOS)
        elseif(UNIX)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_LINUX)
        endif()
    endif()
endfunction()

# Function to set common target properties for Evan
function(set_evan_target_properties TARGET_NAME)
    if(COMMAND xider_apply_target_defaults)
        xider_apply_target_defaults(${TARGET_NAME})
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )

        if(WIN32)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_WINDOWS)
        elseif(APPLE)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_MACOS)
        elseif(UNIX)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_LINUX)
        endif()
    endif()
endfunction()

# Function to set common target properties for Guillaume
function(set_guillaume_target_properties TARGET_NAME)
    if(COMMAND xider_apply_target_defaults)
        xider_apply_target_defaults(${TARGET_NAME})
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )

        if(WIN32)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_WINDOWS)
        elseif(APPLE)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_MACOS)
        elseif(UNIX)
            target_compile_definitions(${TARGET_NAME} PRIVATE OS_LINUX)
        endif()
    endif()
endfunction()
