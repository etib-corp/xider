function(add_doxygen_docs_for_library LIB_NAME LIB_ROOT)
    # Find Doxygen package
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

    # Configure Doxygen settings
    set(DOXYGEN_PROJECT_NAME "${LIB_NAME}")
    set(DOXYGEN_PROJECT_BRIEF "${PROJECT_DESCRIPTION}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")

    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${AWESOME_CSS_DIR}/doxygen-awesome.css")
    set(DOXYGEN_HTML_COLORSTYLE "LIGHT")
    set(DOXYGEN_GENERATE_TREEVIEW YES)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${LIB_ROOT}/README.md")
    # Only parse source and markdown files; treat images as assets via IMAGE_PATH
    set(DOXYGEN_FILE_PATTERNS *.cpp *.hpp *.md *.markdown)
    
    # Configure image paths for assets. Only include existing directories
    set(IMAGE_PATH_CANDIDATES "${LIB_ROOT}/assets" "${LIB_ROOT}/docs/assets" "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_SOURCE_DIR}/docs/assets")
    set(DOXYGEN_IMAGE_PATH)
    foreach(img IN LISTS IMAGE_PATH_CANDIDATES)
        if(EXISTS "${img}")
            list(APPEND DOXYGEN_IMAGE_PATH "${img}")
        endif()
    endforeach()
    
    set(DOXYGEN_EXCLUDE_PATTERNS
        */build/*
        */cmake/*
        */.*
        */CMakeFiles/*
    )

    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_EXTRACT_PRIVATE NO)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_QUIET NO)
    # Treat warnings (including undocumented items) as errors during doc generation
    # This causes the Doxygen build to fail if undocumented public symbols are found
    set(DOXYGEN_WARN_AS_ERROR YES)

    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_INLINE_SOURCES NO)

    # Class diagrams and graphs
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CLASS_DIAGRAMS YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_INCLUDE_GRAPH YES)
    set(DOXYGEN_INCLUDED_BY_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_DOT_MULTI_TARGETS YES)
    
    # Configure image handling
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_STRIP_FROM_PATH "${LIB_ROOT}")
    set(DOXYGEN_STRIP_FROM_INC_PATH "${LIB_ROOT}")

    # Collect source files (headers, docs, examples)
    file(GLOB_RECURSE HEADER_FILES
        "${LIB_ROOT}/headers/*.h"
        "${LIB_ROOT}/headers/*.hh"
        "${LIB_ROOT}/headers/*.hpp"
        "${LIB_ROOT}/headers/*.hxx"
    )
    file(GLOB_RECURSE DOCS_FILES
        "${LIB_ROOT}/docs/*.md"
        "${LIB_ROOT}/docs/**/*.md"
    )
    file(GLOB_RECURSE EXAMPLE_DOCS_FILES
        "${LIB_ROOT}/examples/*.md"
        "${LIB_ROOT}/examples/**/*.md"
    )
    
    # Collect asset files (images, diagrams)
    file(GLOB_RECURSE ASSET_FILES
        "${LIB_ROOT}/assets/*.png"
        "${LIB_ROOT}/assets/*.jpg"
        "${LIB_ROOT}/assets/*.jpeg"
        "${LIB_ROOT}/assets/*.gif"
        "${LIB_ROOT}/assets/*.svg"
        "${LIB_ROOT}/docs/assets/*.png"
        "${LIB_ROOT}/docs/assets/*.jpg"
        "${LIB_ROOT}/docs/assets/*.jpeg"
        "${LIB_ROOT}/docs/assets/*.gif"
        "${LIB_ROOT}/docs/assets/*.svg"
    )

    # Ensure mainpage exists; fall back to project README if missing
    if(EXISTS "${LIB_ROOT}/README.md")
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${LIB_ROOT}/README.md")
    else()
        if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
            set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
        else()
            unset(DOXYGEN_USE_MDFILE_AS_MAINPAGE)
        endif()
    endif()

    # Collect optional documentation files (only if they exist)
    set(OPTIONAL_DOCS)
    if(EXISTS "${LIB_ROOT}/README.md")
        list(APPEND OPTIONAL_DOCS "${LIB_ROOT}/README.md")
    endif()
    if(EXISTS "${LIB_ROOT}/CONTRIBUTING.md")
        list(APPEND OPTIONAL_DOCS "${LIB_ROOT}/CONTRIBUTING.md")
    endif()
    if(NOT EXISTS "${LIB_ROOT}/README.md")
        if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
            list(APPEND OPTIONAL_DOCS "${CMAKE_SOURCE_DIR}/README.md")
        endif()
    endif()

    doxygen_add_docs(
        ${LIB_NAME}_docs
        ${HEADER_FILES}
        ${DOCS_FILES}
        ${EXAMPLE_DOCS_FILES}
        ${OPTIONAL_DOCS}
        ALL
        USE_STAMP_FILE
        COMMENT "Generating API documentation with Doxygen for ${LIB_NAME}"
    )
    
    # Copy asset directories to HTML output for proper image references
    if(EXISTS "${LIB_ROOT}/assets")
        add_custom_command(TARGET ${LIB_NAME}_docs POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
                "${LIB_ROOT}/assets"
                "${CMAKE_CURRENT_BINARY_DIR}/html/assets"
            COMMENT "Copying assets for ${LIB_NAME} documentation"
        )
    endif()
    
    # Copy docs assets to assets/ directory (to match markdown relative paths)
    if(EXISTS "${LIB_ROOT}/docs/assets")
        add_custom_command(TARGET ${LIB_NAME}_docs POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
                "${LIB_ROOT}/docs/assets"
                "${CMAKE_CURRENT_BINARY_DIR}/html/assets"
            COMMENT "Copying documentation assets for ${LIB_NAME}"
        )
    endif()
endfunction()
