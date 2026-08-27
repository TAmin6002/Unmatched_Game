cmake_minimum_required(VERSION 3.11)
project(Unmatched)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- raylib ---
# Vendored raylib source tree lives in "raylib-master"
add_subdirectory(raylib-master)

# --- your game sources ---

file(GLOB GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/test_raylib_main.cpp")

add_executable(Unmatched ${GAME_SOURCES})

add_custom_command(TARGET Unmatched POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Unmatched>/assets)

target_include_directories(Unmatched PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_link_libraries(Unmatched PRIVATE raylib)

# raylib needs these system libs on Linux
if (UNIX AND NOT APPLE)
    target_link_libraries(Unmatched PRIVATE m pthread dl rt X11)
endif()