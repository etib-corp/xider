# Reusable CMake helpers for monorepo projects

include(CMakeParseArguments)

# Apply common project-wide compiler and build defaults.
macro(xider_apply_project_defaults)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()
endmacro()

# Apply common target properties and platform compile definitions.
function(xider_apply_target_defaults TARGET_NAME)
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
endfunction()

# Configure and create a Doxygen docs target with shared settings.
function(xider_add_doxygen_docs TARGET_NAME)
    set(options)
    set(oneValueArgs MAINPAGE COMMENT PROJECT_NAME PROJECT_BRIEF PROJECT_NUMBER)
    set(multiValueArgs INPUTS EXCLUDE_PATTERNS FILE_PATTERNS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_MAINPAGE)
        message(FATAL_ERROR "xider_add_doxygen_docs requires MAINPAGE.")
    endif()
    if(NOT ARG_PROJECT_NAME)
        message(FATAL_ERROR "xider_add_doxygen_docs requires PROJECT_NAME.")
    endif()
    if(NOT ARG_PROJECT_BRIEF)
        message(FATAL_ERROR "xider_add_doxygen_docs requires PROJECT_BRIEF.")
    endif()
    if(NOT ARG_PROJECT_NUMBER)
        message(FATAL_ERROR "xider_add_doxygen_docs requires PROJECT_NUMBER.")
    endif()
    if(NOT ARG_INPUTS)
        message(FATAL_ERROR "xider_add_doxygen_docs requires INPUTS.")
    endif()

    find_package(Doxygen REQUIRED)
    if(NOT Doxygen_FOUND)
        message(FATAL_ERROR "Doxygen not found. Documentation will not be built. Install doxygen and try again.")
    endif()

    find_program(DOT_EXECUTABLE dot)
    if(NOT DOT_EXECUTABLE)
        message(FATAL_ERROR "Graphviz 'dot' executable not found. Please install Graphviz to enable diagram generation in the documentation.")
    endif()

    include(FetchContent)
    FetchContent_Declare(
        doxygen-awesome-css
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
        GIT_TAG v2.4.1
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)
    FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

    set(DOXYGEN_PROJECT_NAME "${ARG_PROJECT_NAME}")
    set(DOXYGEN_PROJECT_BRIEF "${ARG_PROJECT_BRIEF}")
    set(DOXYGEN_PROJECT_NUMBER "${ARG_PROJECT_NUMBER}")

    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${AWESOME_CSS_DIR}/doxygen-awesome.css")
    set(DOXYGEN_HTML_COLORSTYLE "LIGHT")
    set(DOXYGEN_GENERATE_TREEVIEW YES)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${ARG_MAINPAGE}")
    if(ARG_FILE_PATTERNS)
        set(DOXYGEN_FILE_PATTERNS ${ARG_FILE_PATTERNS})
    else()
        set(DOXYGEN_FILE_PATTERNS *.cpp *.hpp *.md)
    endif()

    if(ARG_EXCLUDE_PATTERNS)
        set(DOXYGEN_EXCLUDE_PATTERNS ${ARG_EXCLUDE_PATTERNS})
    else()
        set(DOXYGEN_EXCLUDE_PATTERNS
            */build/*
            */cmake/*
            */.*
            */CMakeFiles/*
        )
    endif()

    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_EXTRACT_PRIVATE NO)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_QUIET NO)
    set(DOXYGEN_WARN_AS_ERROR FAIL_ON_WARNINGS)

    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_INLINE_SOURCES NO)

    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CLASS_DIAGRAMS YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_INCLUDE_GRAPH YES)
    set(DOXYGEN_INCLUDED_BY_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_DOT_MULTI_TARGETS YES)

    if(NOT ARG_COMMENT)
        set(ARG_COMMENT "Generating API documentation with Doxygen")
    endif()

    doxygen_add_docs(
        ${TARGET_NAME}
        ${ARG_INPUTS}
        ALL
        USE_STAMP_FILE
        COMMENT "${ARG_COMMENT}"
    )
endfunction()
