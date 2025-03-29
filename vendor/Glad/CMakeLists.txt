cmake_minimum_required(VERSION 3.16)
project(glad C)

add_library(glad STATIC
    src/glad.c
)

target_include_directories(glad
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT_BUILD)

if(WIN32)
    target_link_libraries(glad PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(glad PRIVATE GL)
endif()

if(MSVC)
    target_compile_options(glad PRIVATE /W3)
else()
    target_compile_options(glad PRIVATE -Wall -Wextra)
endif()

install(TARGETS glad
    EXPORT gladTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)
