cmake_minimum_required(VERSION 3.28)
project(PixelMiner LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(PixelMiner src/main.cxx
)

add_subdirectory(src)
add_subdirectory(externals/minizip-ng)

target_link_libraries(PixelMiner PRIVATE SFML::Graphics SFML::Network SFML::Audio minizip)

target_compile_features(PixelMiner PRIVATE cxx_std_17)
target_compile_definitions(PixelMiner PRIVATE DEBUG=1)

target_include_directories(PixelMiner PRIVATE include/)
target_include_directories(PixelMiner PRIVATE externals/minizip-ng)

target_precompile_headers(PixelMiner PRIVATE include/stdafx.hxx)

if(WIN32)
    add_custom_command(
        TARGET PixelMiner
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:pixelminer>
        VERBATIM)
endif()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/src/pixelminer/"
        "${CMAKE_BINARY_DIR}/bin/pixelminer"
    COMMENT "Copying Assets folder"
)

add_dependencies(PixelMiner copy_assets)

install(TARGETS PixelMiner)
