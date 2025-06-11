cmake_minimum_required(VERSION 3.25)
project(
    "Bouncing_DVD_logo"
    VERSION 0.1.0
    LANGUAGES CXX
)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES "src/*.c*")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23 c_std_11)

if (NOT TARGET UveDX)
  find_package(UveDX REQUIRED)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    UveDX
)

target_include_directories(${PROJECT_NAME} PRIVATE "include")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/DVD.png"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/DVD.png"
)
