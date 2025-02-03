cmake_minimum_required(VERSION 3.28)
project(pixelminer LANGUAGES CXX)

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

add_executable(pixelminer src/main.cxx)

add_subdirectory(src)
add_subdirectory(externals/minizip-ng)

target_link_libraries(pixelminer PRIVATE SFML::Graphics SFML::Network SFML::Audio minizip)

target_compile_features(pixelminer PRIVATE cxx_std_17)
target_compile_definitions(pixelminer PRIVATE DEBUG=1 GAME_VERSION="Alpha 0.3.0" METADATA_VERSION="1.0.0" TERRAIN_GENERATOR_VERSION="1.1.0")

target_include_directories(pixelminer PRIVATE include/)
target_include_directories(pixelminer PRIVATE externals/minizip-ng)

target_precompile_headers(pixelminer PRIVATE include/stdafx.hxx)

if(WIN32)
    add_custom_command(
        TARGET pixelminer
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:pixelminer>
        VERBATIM)
endif()

install(TARGETS pixelminer)