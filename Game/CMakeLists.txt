cmake_minimum_required(VERSION 3.25)
project(
    "ChickenInvaders1"
    VERSION 0.7.1
    LANGUAGES CXX
)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WITH_DISCORD "Enable Discord integration" OFF)

if(WITH_DISCORD)
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        set(DISCORD_ARCH "x86")
    else()
        set(DISCORD_ARCH "x86_64")
    endif()

    find_package(discord_game_sdk QUIET)
    if (NOT discord_game_sdk_FOUND)
        FetchContent_Declare(
            discord_game_sdk
            URL https://dl-game-sdk.discordapp.net/latest/discord_game_sdk.zip
        )
        FetchContent_GetProperties(discord_game_sdk)
        if(NOT discord_game_sdk_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_MakeAvailable(discord_game_sdk)
        endif()

        set(DISCORD_INCLUDE_PATH "${discord_game_sdk_SOURCE_DIR}/cpp")
        set(DISCORD_LIB_PATH "${discord_game_sdk_SOURCE_DIR}/lib/${DISCORD_ARCH}")
        set(DISCORD_SDK_SOURCE_LIB "${DISCORD_LIB_PATH}/discord_game_sdk${CMAKE_SHARED_LIBRARY_SUFFIX}")

        file(GLOB_RECURSE DISCORD_CPP_WRAPPER_SOURCES "${discord_game_sdk_SOURCE_DIR}/cpp/*.cpp")

        add_library(discord_cpp_wrapper STATIC ${DISCORD_CPP_WRAPPER_SOURCES})
        target_include_directories(discord_cpp_wrapper 
            PUBLIC "${DISCORD_INCLUDE_PATH}"
        )
        
        # Force include cstdint for compilers that support this feature
        if(MSVC)
            target_compile_options(discord_cpp_wrapper PRIVATE "/FIcstdint")
        else()
            target_compile_options(discord_cpp_wrapper PRIVATE -include cstdint)
        endif()

        add_library(discord_shared SHARED IMPORTED)
        set_target_properties(discord_shared PROPERTIES
            IMPORTED_LOCATION "${DISCORD_SDK_SOURCE_LIB}"
        )

        if(WIN32)
            set_target_properties(discord_shared PROPERTIES
                IMPORTED_IMPLIB "${DISCORD_LIB_PATH}/discord_game_sdk.dll.lib"
            )
        endif()

        target_link_libraries(discord_cpp_wrapper INTERFACE discord_shared)
    endif()
endif()

file(GLOB_RECURSE GAME_SRC "src/*.c*")

add_executable(${PROJECT_NAME} ${GAME_SRC})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23 c_std_11)

if(WITH_DISCORD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_DISCORD)
endif()

if (NOT TARGET UveDX)
  find_package(UveDX REQUIRED)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    UveDX
    $<$<BOOL:${WITH_DISCORD}>:discord_cpp_wrapper>
)

target_include_directories(${PROJECT_NAME} PRIVATE "include")

if(WITH_DISCORD)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DISCORD_SDK_SOURCE_LIB}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/ChickenInvaders.dat"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ChickenInvaders.dat"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive /we4062 /wd4244 /wd4267 /wd4458 /wd6387 /wd26495 /wd4996)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic -Wswitch-enum)
endif()
