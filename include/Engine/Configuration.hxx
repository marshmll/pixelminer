#pragma once

constexpr unsigned int GRID_SIZE = 16;

#ifndef GAME_VERSION
#warning "GAME_VERSION NOT DEFINED IN COMPILE TILE, CHECK target_compile_definitions"
#endif

#ifndef METADATA_VERSION
#warning "METADATA_VERSION NOT DEFINED IN COMPILE TILE, CHECK target_compile_definitions"
#endif

#ifndef TERRAIN_GENERATOR_VERSION
#warning "TERRAIN_GENERATOR_VERSION NOT DEFINED IN COMPILE TILE, CHECK target_compile_definitions"
#endif

#ifdef DEBUG
#define GLOBAL_FOLDER static_cast<const std::string>("Assets/")

#elif __linux__
#define GLOBAL_FOLDER static_cast<const std::string>(getenv("HOME")) + "/.pixelminer/"

#elif __WIN32__
#define GLOBAL_FOLDER static_cast<const std::string>(getenv("AppData") + "/.pixelminer/")
#endif

#ifndef GLOBAL_FOLDER
#error "UNSUPORTED TARGET MACHINE: WILL NOT COMPILE"
#endif

#ifndef SETTINGS_FOLDER
#define SETTINGS_FOLDER GLOBAL_FOLDER + "Settings/"
#endif

#ifndef MAPS_FOLDER
#define MAPS_FOLDER GLOBAL_FOLDER + "Maps/"
#endif

#ifndef RESOURCES_FOLDER
#define RESOURCES_FOLDER GLOBAL_FOLDER + "ResourcePacks/"
#endif