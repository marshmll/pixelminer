#pragma once

constexpr unsigned int GRID_SIZE = 16;

#ifndef GAME_VERSION
#define GAME_VERSION "Alpha 0.11.1"
#endif

#ifndef METADATA_VERSION
#define METADATA_VERSION "1.0.0"
#endif

#ifndef TERRAIN_GENERATOR_VERSION
#define TERRAIN_GENERATOR_VERSION "1.2.0"
#endif

#ifdef DEBUG
#define GLOBAL_FOLDER static_cast<const std::string>(".pixelminer/")

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

#ifndef CACHE_FOLDER
#define CACHE_FOLDER GLOBAL_FOLDER + ".cache/"
#endif