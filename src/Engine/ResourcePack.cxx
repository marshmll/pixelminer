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
        logger.logError("Inexistent resouce pack: " + root_path.string());

    if (!std::filesystem::exists(root_path / "pack.json"))
        logger.logError("Resource pack \"" + root_path.string() + "\" does not have a \"pack.json\" file.");

    std::ifstream packFile(root_path / "pack.json");

    if (!packFile.is_open())
        logger.logError("Failed to open \"pack.json\" file in resource pack: " + root_path.string());

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

    if (!textures["HeartEmpty"].loadFromFile(root_path / "Images/Icons/heart_empty.png"))
        logger.logError("Missing texture \"heart_empty.png\" in resource pack: " + name);

    if (!textures["HeartHalf"].loadFromFile(root_path / "Images/Icons/heart_half.png"))
        logger.logError("Missing texture \"heart_half.png\" in resource pack: " + name);

    if (!textures["HeartFull"].loadFromFile(root_path / "Images/Icons/heart_full.png"))
        logger.logError("Missing texture \"heart_full.png\" in resource pack: " + name);

    if (!textures["BreadEmpty"].loadFromFile(root_path / "Images/Icons/bread_empty.png"))
        logger.logError("Missing texture \"bread_empty.png\" in resource pack: " + name);

    if (!textures["BreadHalf"].loadFromFile(root_path / "Images/Icons/bread_half.png"))
        logger.logError("Missing texture \"bread_half.png\" in resource pack: " + name);

    if (!textures["BreadFull"].loadFromFile(root_path / "Images/Icons/bread_full.png"))
        logger.logError("Missing texture \"bread_full.png\" in resource pack: " + name);

    // Ensure that texture smoothing is off
    for (auto &[_, texture] : textures)
        texture.setSmooth(false);

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

    logger.logInfo("Resource pack \"" + name + "\" sucessfully loaded:\n    - " + std::to_string(textures.size()) +
                   " textures loaded\n    - " + std::to_string(fonts.size()) + " fonts loaded");
}

sf::Texture &ResourcePack::getTexture(const std::string &key)
{
    try
    {
        return textures.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError("Inexistent texture \"" + key + "\" in resource pack" + name);
    }

    return textures.at(key); // SHOULD NEVER REACH HERE!
}