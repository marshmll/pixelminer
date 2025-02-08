/**
 * @file Metadata.hxx
 * @brief Declares the Metadata struct to manage world metadata.
 */

#pragma once

#include "Tools/JSON.hxx"

/**
 * @struct Metadata
 * @brief Structure to hold metadata information related to the map, such as game version, creation date, and other
 * related attributes.
 *
 * This structure contains various data fields that describe a map's metadata, including its version, game version,
 * creation date, difficulty, seed, and more. The metadata can be serialized and deserialized using JSON format.
 */
struct Metadata
{
    /**
     * @struct DataPacks
     * @brief Structure to hold enabled and disabled data packs for the map.
     *
     * This structure contains two arrays: one for enabled data packs and one for disabled data packs.
     * These can be modified or queried for the state of each data pack.
     */
    struct DataPacks
    {
        JArray enabled;  ///< Array of enabled data packs.
        JArray disabled; ///< Array of disabled data packs.
    };

    std::string metadataVersion; ///< Version of the metadata.
    std::string gameVersion;     ///< Version of the game associated with this map.
    long long creationDate;      ///< The creation date of the map (in epoch time).
    DataPacks dataPacks;         ///< Data packs information, including enabled and disabled packs.
    long long dayTime;           ///< The current time of day in the map (in epoch time).
    std::string difficulty;      ///< The difficulty setting of the map.
    std::string generatorName;   ///< Name of the generator used for the map.
    long long lastPlayed;        ///< The last played timestamp of the map (in epoch time).
    std::string name;            ///< Name of the map.
    long long seed;              ///< Seed used for the generation of the map.
    long long spawnX;            ///< X-coordinate of the spawn point.
    long long spawnY;            ///< Y-coordinate of the spawn point.
    double timePlayed;           ///< Total time played on the map (in hours).
};

/**
 * @brief Serializes the given Metadata object to a JObject.
 *
 * This function converts the Metadata object into a JSON object for serialization. It includes various attributes
 * such as metadataVersion, gameVersion, dayTime, spawn coordinates, etc., and organizes them in a structured manner.
 *
 * @param obj The JObject to which the metadata is serialized.
 * @param metadata The Metadata object to serialize.
 * @return The serialized JObject containing the metadata information.
 */
inline JObject &operator<<(JObject &obj, Metadata &metadata)
{
    obj = JObject{{"metadataVersion", metadata.metadataVersion},
                  {"gameVersion", metadata.gameVersion},
                  {"dataPacks",
                   JObject{
                       {"enabled", metadata.dataPacks.enabled},
                       {"disabled", metadata.dataPacks.disabled},
                   }},
                  {"dayTime", metadata.dayTime},
                  {"name", metadata.name},
                  {"difficulty", metadata.difficulty},
                  {"seed", metadata.seed},
                  {"generatorName", metadata.generatorName},
                  {"creationDate", metadata.creationDate},
                  {"lastPlayed", metadata.lastPlayed},
                  {"spawnX", metadata.spawnX},
                  {"spawnY", metadata.spawnY},
                  {"timePlayed", metadata.timePlayed}};
    return obj;
}

/**
 * @brief Deserializes a JObject to populate the given Metadata object.
 *
 * This function extracts data from a JSON object and populates the fields of the Metadata object.
 * It expects the JSON to contain all the necessary attributes (like metadataVersion, gameVersion, dayTime, etc.)
 * and assigns them to the corresponding fields in the Metadata object.
 *
 * @param obj The JObject from which to deserialize the metadata.
 * @param metadata The Metadata object to populate.
 * @return The updated JObject after deserializing the data.
 */
inline JObject &operator>>(JObject &obj, Metadata &metadata)
{
    metadata.metadataVersion = obj.at("metadataVersion").getAs<std::string>();
    metadata.gameVersion = obj.at("gameVersion").getAs<std::string>();
    metadata.creationDate = obj.at("creationDate").getAs<long long>();
    metadata.dataPacks.enabled = obj.at("dataPacks").getAs<JObject>().at("enabled").getAs<JArray>();
    metadata.dataPacks.disabled = obj.at("dataPacks").getAs<JObject>().at("disabled").getAs<JArray>();
    metadata.dayTime = obj.at("dayTime").getAs<long long>();
    metadata.difficulty = obj.at("difficulty").getAs<std::string>();
    metadata.generatorName = obj.at("generatorName").getAs<std::string>();
    metadata.lastPlayed = obj.at("lastPlayed").getAs<long long>();
    metadata.name = obj.at("name").getAs<std::string>();
    metadata.seed = obj.at("seed").getAs<long long>();
    metadata.spawnX = obj.at("spawnX").getAs<long long>();
    metadata.spawnY = obj.at("spawnY").getAs<long long>();
    metadata.timePlayed = obj.at("timePlayed").getAs<double>();

    return obj;
}
