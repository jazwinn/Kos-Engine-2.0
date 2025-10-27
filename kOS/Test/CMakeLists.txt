cmake_minimum_required(VERSION 3.5)
project(Kos_test)

# Collect all test sources
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Create test executable
add_executable(${PROJECT_NAME} ${TEST_SOURCES})

# Add GTest from dependencies (stable release recommended, e.g., release-1.12.1)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/googletest)

# Link with your engine and GTest (do NOT use gtest_main)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Kos_Engine
        gtest
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(Kos_test PRIVATE /Zc:preprocessor)
endif()

# Enable testing
enable_testing()
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
