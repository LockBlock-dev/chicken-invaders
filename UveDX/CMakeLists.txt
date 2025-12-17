cmake_minimum_required(VERSION 3.25)
project(
    "UveDX"
    VERSION 0.7.1
    LANGUAGES CXX
)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML COMPONENTS graphics audio CONFIG QUIET)
if (NOT SFML_FOUND)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    FetchContent_GetProperties(SFML)
    if(NOT SFML_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(SFML)
    endif()
endif()

find_package(tinyfiledialogs QUIET)
if (NOT tinyfiledialogs_FOUND)
    FetchContent_Declare(
        tinyfiledialogs
        GIT_REPOSITORY http://git.code.sf.net/p/tinyfiledialogs/code
        GIT_TAG 5fdb74764a5026d059517911fc085a1cd9da8cee
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    FetchContent_GetProperties(tinyfiledialogs)
    if(NOT tinyfiledialogs_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(tinyfiledialogs)

        add_library(tinyfiledialogs STATIC
            ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c
        )

        target_include_directories(tinyfiledialogs PUBLIC
            ${tinyfiledialogs_SOURCE_DIR}
        )
    endif()
endif()

file(GLOB_RECURSE UVEDX_SRC_FILES "src/*.c*")
file(GLOB_RECURSE UVEDX_HDR_FILES "include/UveDX/*.h*")

add_library(${PROJECT_NAME} ${UVEDX_SRC_FILES} ${UVEDX_HDR_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23 c_std_11)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    SFML::Graphics
    SFML::Audio
  PRIVATE
    tinyfiledialogs
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive /we4062 /wd4244 /wd4267 /wd4458 /wd6387 /wd26495 /wd4996)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic -Wswitch-enum)
endif()
