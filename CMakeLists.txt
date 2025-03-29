cmake_minimum_required(VERSION 3.16)
project(Cinix VERSION 0.1.0 LANGUAGES C)

if(NOT (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Linux"))
    message(FATAL_ERROR "Unsupported platform. Only Windows and Linux are supported.")
endif()

if(CMAKE_SIZEOF_VOID_P LESS 8)
    message(FATAL_ERROR "Only x64 architecture is supported.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(CINIX_BUILD_TESTS "Build the test suite" OFF)

set(SDL_SHARED TRUE)
add_subdirectory(vendor/SDL)

set(SDL3MIXER_VENDORED TRUE)
set(SDL3MIXER_SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL" CACHE PATH "SDL3 source directory")
add_subdirectory(vendor/SDL_mixer)

set(SDL3TTF_VENDORED TRUE)
set(SDL3TTF_SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL" CACHE PATH "SDL3 source directory")
add_subdirectory(vendor/SDL_ttf)

add_subdirectory(vendor/Glad)

file(GLOB_RECURSE ENGINE_SOURCES "src/engine/*.c")
file(GLOB_RECURSE ENGINE_HEADERS "src/engine/*.h")

add_executable(cinix ${ENGINE_SOURCES} "src/main.c")

target_include_directories(cinix PRIVATE
    src
    src/engine
    ${CMAKE_BINARY_DIR}/include
)

target_link_libraries(cinix PRIVATE
    SDL3::SDL3
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
    glad
)

if(WIN32)
    target_compile_definitions(cinix PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
    
    if(MSVC)
        target_compile_options(cinix PRIVATE /W4 /wd4996)
    endif()
    
elseif(UNIX AND NOT APPLE)
    target_compile_options(cinix PRIVATE -Wall -Wextra)
    target_link_libraries(cinix PRIVATE m dl pthread)
endif()

add_custom_command(TARGET cinix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:cinix>/assets
)

add_custom_command(TARGET cinix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:cinix>/shaders
)

install(TARGETS cinix
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/ DESTINATION bin/assets)
install(DIRECTORY shaders/ DESTINATION bin/shaders)
