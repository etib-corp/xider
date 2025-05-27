message(STATUS "Windows configuration loaded")
set(OS_DEFINE OS_WINDOWS)

foreach(flag_var
    CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG)
    string(REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
endforeach()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")