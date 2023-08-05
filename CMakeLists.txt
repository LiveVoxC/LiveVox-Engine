cmake_minimum_required(VERSION 3.21)
project(CMakeSFMLProject LANGUAGES CXX)
include(FetchContent)

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

add_executable(CMakeSFMLProject src/main.cpp)

target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics EnTT)

target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET CMakeSFMLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS CMakeSFMLProject)