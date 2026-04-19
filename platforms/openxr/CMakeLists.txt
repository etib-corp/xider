# Build evan for OpenXR
set(BUILD_FOR_OPENXR ON)

#8 Add the platform-specific subdirectories
add_subdirectory(android)
add_subdirectory(linux)