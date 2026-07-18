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
# WIN32 prevents the console/terminal window from appearing
add_executable(${PROJECT_NAME} WIN32
    ${SRC_FILES}
    icon.rc
)
# Link Raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets folder next to the executable after building
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# Copy required DLL files next to the executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/msys64/ucrt64/bin/libraylib.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/msys64/ucrt64/bin/glfw3.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)