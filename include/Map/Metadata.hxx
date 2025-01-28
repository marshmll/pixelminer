#pragma once

#include "Tools/JSON.hxx"

struct Metadata
{
    struct DataPacks
    {
        JArray enabled;
        JArray disabled;
    };

    std::string metadataVersion;
    std::string gameVersion;
    long long creationDate;
    DataPacks dataPacks;
    long long dayTime;
    std::string difficulty;
    std::string generatorName;
    long long lastPlayed;
    std::string name;
    long long seed;
    long long spawnX;
    long long spawnY;
    double timePlayed;
};

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