cmake_minimum_required(VERSION 3.15)

project(BounceGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Raylib
find_package(raylib REQUIRED)

# Include header files
include_directories(include)

# Automatically find all .cpp files in src/
file(GLOB SRC_FILES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link Raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets folder after building
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

