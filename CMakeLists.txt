cmake_minimum_required(VERSION 3.21)
project(LiveVoxEngine LANGUAGES CXX)
include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.12.2)

set(ENTT_BUILD_TEST OFF CACHE BOOL "Disable EnTT tests")
set(ENTT_BUILD_EXAMPLES OFF CACHE BOOL "Disable EnTT examples")

FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

FetchContent_MakeAvailable(SFML) 

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(LiveVoxEngine ${SOURCES})

# Fix MSVC AMD: prevent compiler from halting when warnings occurs
if(MSVC)
    target_compile_options(sfml-graphics PRIVATE /w)
endif()

target_include_directories(LiveVoxEngine PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(LiveVoxEngine PRIVATE 
    sfml-graphics 
    EnTT
)

target_compile_features(LiveVoxEngine PRIVATE cxx_std_17)

# Fix Clangd: force cmake to append -std=c++17 into compile_commands.json
set_target_properties(LiveVoxEngine PROPERTIES CXX_STANDARD 17)


if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET LiveVoxEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:LiveVoxEngine> $<TARGET_FILE_DIR:LiveVoxEngine> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS LiveVoxEngine)
