#pragma onnce

#include "Tools/JSON.hxx"

struct Metadata
{
    struct DataPacks
    {
        JSONArray enabled;
        JSONArray disabled;
    };

    std::string dataVersion;
    std::string gameVersion;
    long long creationTime;
    DataPacks dataPacks;
    long long dayTime;
    std::string difficulty;
    std::string generatorName;
    long long lastPlayed;
    std::string name;
    long long seed;
    long long spawnX;
    long long spawnY;
    long long timePlayed;
};

inline JSONObject &operator<<(JSONObject &obj, Metadata &metadata)
{
    obj = JSONObject{{"dataVersion", metadata.dataVersion},
                     {"gameVersion", metadata.gameVersion},
                     {"dataPacks",
                      JSONObject{
                          {"enabled", metadata.dataPacks.enabled},
                          {"disabled", metadata.dataPacks.disabled},
                      }},
                     {"dayTime", metadata.dayTime},
                     {"name", metadata.name},
                     {"difficulty", metadata.difficulty},
                     {"seed", metadata.seed},
                     {"generatorName", metadata.generatorName},
                     {"creationTime", metadata.creationTime},
                     {"lastPlayed", metadata.lastPlayed},
                     {"spawnX", metadata.spawnX},
                     {"spawnY", metadata.spawnY},
                     {"timePlayed", metadata.timePlayed}};

    return obj;
}

inline JSONObject &operator>>(JSONObject &obj, Metadata &metadata)
{
    metadata.dataVersion = obj.at("dataVersion").get<std::string>();
    metadata.gameVersion = obj.at("gameVersion").get<std::string>();
    metadata.creationTime = obj.at("creationTime").get<long long>();
    metadata.dataPacks.enabled = obj.at("dataPacks").get<JSONObject>().at("enabled").get<JSONArray>();
    metadata.dataPacks.disabled = obj.at("dataPacks").get<JSONObject>().at("disabled").get<JSONArray>();
    metadata.dayTime = obj.at("dayTime").get<long long>();
    metadata.difficulty = obj.at("difficulty").get<std::string>();
    metadata.generatorName = obj.at("generatorName").get<std::string>();
    metadata.lastPlayed = obj.at("lastPlayed").get<long long>();
    metadata.name = obj.at("name").get<std::string>();
    metadata.seed = obj.at("seed").get<long long>();
    metadata.spawnX = obj.at("spawnX").get<long long>();
    metadata.spawnY = obj.at("spawnY").get<long long>();

    return obj;
}