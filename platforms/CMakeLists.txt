# Automatically discover and add all platform-specific directories
file(GLOB PLATFORM_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(dir ${PLATFORM_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
        message(STATUS "Found platform directory: ${dir}")
        add_subdirectory(${dir})
    endif()
endforeach()
