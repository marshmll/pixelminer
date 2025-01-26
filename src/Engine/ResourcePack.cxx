#include "Engine/ResourcePack.hxx"
#include "stdafx.hxx"

ResourcePack::ResourcePack() : logger("ResourcePack")
{
}

void ResourcePack::load(const std::string &name)
{
    std::filesystem::path root_path;
    root_path = RESOURCES_FOLDER + name;

    if (!std::filesystem::exists(root_path))
        logger.logError("Inexistent resouce pack: " + name);

    if (!std::filesystem::exists(root_path / "pack.json"))
        logger.logError("Resource pack \"" + name + "\" does not have a \"pack.json\" file.");

    std::ifstream packFile(root_path / "pack.json");

    if (!packFile.is_open())
        logger.logError("Failed to open \"pack.json\" file in resource pack: " + name);

    std::stringstream packStream;
    packStream << packFile.rdbuf();

    JObject packObj = JSON::parse(packStream.str()).getAs<JObject>();

    packFile.close();

    /* Name and description +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    this->name = packObj.at("name").getAs<std::string>();
    this->description = packObj.at("description").getAs<std::string>();

    /* Icon +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!this->icon.loadFromFile(root_path / "icon.png"))
        logger.logError("Missing texture \"icon.png\" in resource pack: " + name);

    /* Fonts ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    for (auto const &entry : std::filesystem::directory_iterator{root_path / "Fonts"})
    {
        std::string filename = entry.path().filename();
        std::string key = filename.substr(0, filename.size() - 4);

        if (!this->fonts[key].openFromFile(entry.path()))
            logger.logError("Failed to open font file: " + entry.path().string() + " from resource pack: " + name);
    }

    /* Default textures +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!textures["Background"].loadFromFile(root_path / "Images/Backgrounds/background.png"))
        logger.logError("Missing texture \"background.png\" in resource pack: " + name);

    if (!textures["Player1"].loadFromFile(root_path / "Images/Sprites/Entities/Player/player_1.png"))
        logger.logError("Missing texture \"player_1.png\" in resource pack: " + name);

    if (!textures["TileSheet"].loadFromFile(root_path / "Images/Tiles/tile_sheet.png"))
        logger.logError("Missing texture \"tile_sheet.png\" in resource pack: " + name);

    /* Tile Data ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    std::ifstream tileDataFile(root_path / "tiledata.json");

    if (!tileDataFile.is_open())
        logger.logError("Failed to open \"tiledata.json\" file in resource pack: " + name);

    std::stringstream tileDataStream;
    tileDataStream << tileDataFile.rdbuf();

    JArray tileDataArray = JSON::parse(tileDataStream.str()).getAs<JArray>();

    for (auto &entry : tileDataArray)
    {
        JObject obj = entry.getAs<JObject>();

        std::string id, name;
        int rect_x, rect_y, size;

        id = obj.at("id").getAs<std::string>();
        name = obj.at("name").getAs<std::string>();
        rect_x = static_cast<int>(obj.at("rectX").getAs<long long>());
        rect_y = static_cast<int>(obj.at("rectY").getAs<long long>());
        size = static_cast<int>(obj.at("size").getAs<long long>());

        // std::cout << id << "\n";

        this->tileDB[id] = TileData{id, name, sf::IntRect({rect_x * size, rect_y * size}, {size, size}), size};
    }

    logger.logInfo("Resource pack \"" + name + "\" sucessfully loaded.");
}
