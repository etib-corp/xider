# Set test directories
set(TEST_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(TEST_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Find all source files in the test sources dir
file(GLOB_RECURSE TEST_SOURCES "${TEST_SOURCES_DIR}/*.cpp")

# Add the executable target
add_executable(test_${PROJECT_NAME}
    ${TEST_SOURCES}
    ${SOURCES}
)

# Set the target properties
set_target_properties(test_${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Set compilation prepocessor definitions
target_compile_definitions(test_${PROJECT_NAME} PRIVATE
    TESTING
)

# Include test headers
target_include_directories(test_${PROJECT_NAME} PRIVATE
    ${TEST_HEADERS_DIR}
)

# Include the main project headers
target_include_directories(test_${PROJECT_NAME} PRIVATE
    ${HEADERS_DIR}
)

# Link GoogleTest libraries
target_link_libraries(test_${PROJECT_NAME} PRIVATE
    gtest_main
)

# Discover tests using GoogleTest
include(GoogleTest)
gtest_discover_tests(test_${PROJECT_NAME})
