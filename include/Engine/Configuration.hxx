#pragma once

/**
 * @brief Constant defining the grid size for the game.
 *
 * This constant defines the size of the grid used in the game. The grid is utilized for
 * managing the terrain and other spatial aspects in the game.
 */
constexpr unsigned int GRID_SIZE = 16;

/**
 * @brief Defines the version of the game.
 *
 * The `GAME_VERSION` is the current version of the game. If not already defined elsewhere,
 * it will be set to the given value.
 */
#ifndef GAME_VERSION
#define GAME_VERSION "Alpha 0.11.3"
#endif

/**
 * @brief Defines the version of the metadata.
 *
 * The `METADATA_VERSION` specifies the current version of the game's metadata. If not already
 * defined elsewhere, it will be set to the given value.
 */
#ifndef METADATA_VERSION
#define METADATA_VERSION "1.0.0"
#endif

/**
 * @brief Defines the version of the terrain generator.
 *
 * The `TERRAIN_GENERATOR_VERSION` defines the current version of the terrain generator. If not
 * already defined elsewhere, it will be set to the given value.
 */
#ifndef TERRAIN_GENERATOR_VERSION
#define TERRAIN_GENERATOR_VERSION "1.2.0"
#endif

/**
 * @brief Specifies the global folder path for different platforms.
 *
 * The `GLOBAL_FOLDER` macro defines the path where game-related data will be stored.
 * It is platform-dependent and will vary based on whether the platform is `DEBUG`, `__linux__`, or `__WIN32__`.
 *
 * - For `DEBUG`, the path is set to ".pixelminer/".
 * - For `__linux__`, it is set to the user's home directory + "/.pixelminer/".
 * - For `__WIN32__`, it is set to the "AppData" directory + "/.pixelminer/".
 *
 * If none of these platforms match, the code will fail to compile.
 */
#ifdef DEBUG
#define GLOBAL_FOLDER static_cast<const std::string>(".pixelminer/")

#elif __linux__
#define GLOBAL_FOLDER static_cast<const std::string>(getenv("HOME")) + "/.pixelminer/"

#elif __WIN32__
#define GLOBAL_FOLDER static_cast<const std::string>(getenv("AppData") + "/.pixelminer/")
#endif

/**
 * @brief Error if no supported platform is detected.
 *
 * If the platform does not match any of the defined ones (`DEBUG`, `__linux__`, or `__WIN32__`),
 * this will generate a compile-time error.
 */
#ifndef GLOBAL_FOLDER
#error "UNSUPPORTED TARGET MACHINE: WILL NOT COMPILE"
#endif

/**
 * @brief Defines the folder path for storing settings.
 *
 * The `SETTINGS_FOLDER` defines the directory path where the game settings will be stored.
 * This is based on the `GLOBAL_FOLDER` path, which is platform-dependent.
 */
#ifndef SETTINGS_FOLDER
#define SETTINGS_FOLDER GLOBAL_FOLDER + "Settings/"
#endif

/**
 * @brief Defines the folder path for storing maps.
 *
 * The `MAPS_FOLDER` defines the directory path where the game's map data will be stored.
 * This is based on the `GLOBAL_FOLDER` path, which is platform-dependent.
 */
#ifndef MAPS_FOLDER
#define MAPS_FOLDER GLOBAL_FOLDER + "Maps/"
#endif

/**
 * @brief Defines the folder path for storing resource packs.
 *
 * The `RESOURCES_FOLDER` defines the directory path where the game’s resource packs will be stored.
 * This is based on the `GLOBAL_FOLDER` path, which is platform-dependent.
 */
#ifndef RESOURCES_FOLDER
#define RESOURCES_FOLDER GLOBAL_FOLDER + "ResourcePacks/"
#endif

/**
 * @brief Defines the folder path for storing cached data.
 *
 * The `CACHE_FOLDER` defines the directory path where temporary or cached data will be stored.
 * This is based on the `GLOBAL_FOLDER` path, which is platform-dependent.
 */
#ifndef CACHE_FOLDER
#define CACHE_FOLDER GLOBAL_FOLDER + ".cache/"
#endif
