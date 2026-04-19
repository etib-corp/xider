# Build evan for GLFW
set(BUILD_FOR_GLFW ON)

#8 Add the platform-specific subdirectories
add_subdirectory(linux)
add_subdirectory(macos)
add_subdirectory(windows)