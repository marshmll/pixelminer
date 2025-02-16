#include "Engine/ResourcePack.hxx"
#include "stdafx.hxx"

ResourcePack::ResourcePack() : logger("ResourcePack")
{}

ResourcePack::~ResourcePack() = default;

const bool ResourcePack::load(const std::string &name)
{
    if (!std::filesystem::exists(CACHE_FOLDER + "ResourcePacks/"))
        std::filesystem::create_directories(CACHE_FOLDER + "ResourcePacks/");

    std::filesystem::path root_path = CACHE_FOLDER + "ResourcePacks/" + name;

    if (!std::filesystem::exists(root_path))
    {
        if (!std::filesystem::exists(RESOURCES_FOLDER + name + ".zip"))
        {
            logger.logError(_("Inexistent resouce pack: ") + root_path.string(), false);
            return false;
        }

        if (!Zip::extract(RESOURCES_FOLDER + name + ".zip", CACHE_FOLDER + "ResourcePacks/"))
        {
            logger.logError(_("Failed to cache resource pack: ") + name, false);
            return false;
        }
    }

    if (!std::filesystem::exists(root_path / "pack.json"))
    {
        logger.logError(_("Resource pack ") + root_path.string() + _(" does not have a \"pack.json\" file."), false);
        return false;
    }

    std::ifstream packFile(root_path / "pack.json");

    if (!packFile.is_open())
    {
        logger.logError(_("Failed to open \"pack.json\" file in resource pack: ") + root_path.string(), false);
        return false;
    }

    std::stringstream packStream;
    packStream << packFile.rdbuf();

    JObject packObj = JSON::parse(packStream.str()).getAs<JObject>();

    packFile.close();

    /* Name and description +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    this->name = packObj.at("name").getAs<std::string>();
    this->description = packObj.at("description").getAs<std::string>();

    /* Icon +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    if (!this->icon.loadFromFile(root_path / "icon.png"))
    {
        logger.logError(_("Missing texture \"icon.png\" in resource pack: ") + name, false);
        return false;
    }

    /* Fonts ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    for (auto const &entry : std::filesystem::directory_iterator{root_path / "Assets/Fonts"})
    {
        std::string filename = entry.path().filename();
        std::string key = filename.substr(0, filename.size() - 4);

        if (!this->fonts[key].openFromFile(entry.path()))
        {
            logger.logError(_("Failed to open font file: ") + entry.path().string() + _(" from resource pack: ") + name,
                            false);
            return false;
        }

        this->fonts.at(key).setSmooth(false);
    }

    /* Default images +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    std::ifstream images_file(root_path / "Assets/Textures/textures.json");

    if (!images_file.is_open())
    {
        logger.logError(_("Failed to open \"textures.json\" file in resource pack: ") + root_path.string(), false);
        return false;
    }

    std::stringstream ss;
    ss << images_file.rdbuf();
    images_file.close();

    JArray images_array = JSON::parse(ss.str()).getAs<JArray>();

    // Load textures from images.json
    for (auto &val : images_array)
    {
        JObject entry = val.getAs<JObject>();
        std::string key = entry.at("key").getAs<std::string>(), path = entry.at("path").getAs<std::string>();

        if (!textures[key].loadFromFile(root_path / path))
            logger.logWarning(_("Missing texture ") + path + _(" in resource pack: ") + name);

        // Ensure that texture smoothing is off
        textures.at(key).setSmooth(false);
    }

    /* Tile Database ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    std::ifstream tile_data_file(root_path / "tile_db.json");

    if (!tile_data_file.is_open())
        logger.logError(_("Failed to open \"tile_db.json\" file in resource pack: ") + name, false);

    std::stringstream tile_data_stream;
    tile_data_stream << tile_data_file.rdbuf();

    JArray tile_data_array = JSON::parse(tile_data_stream.str()).getAs<JArray>();

    for (auto &entry : tile_data_array)
    {
        try
        {
            JObject obj = entry.getAs<JObject>();

            std::string id, name;
            int rect_x, rect_y, size;

            id = obj.at("id").getAs<std::string>();
            name = obj.at("name").getAs<std::string>();
            rect_x = static_cast<int>(obj.at("rectX").getAs<long long>());
            rect_y = static_cast<int>(obj.at("rectY").getAs<long long>());
            size = static_cast<int>(obj.at("size").getAs<long long>());

            tileDB[id] = TileData{id, name, sf::IntRect({rect_x * size, rect_y * size}, {size, size}), size};
        }
        catch (std::runtime_error &e)
        {
            logger.logError(_("Error while reading tile database: ") + static_cast<std::string>(e.what()), false);
            return false;
        }
    }

    logger.logInfo(_("Sucessfully loaded resource pack ") + name + ": " + std::to_string(textures.size()) +
                   _(" textures, ") + std::to_string(fonts.size()) + _(" fonts."));

    return true;
}

sf::Font &ResourcePack::getFont(const std::string &key)
{
    try
    {
        return fonts.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent font ") + key + _(" in resource pack") + name);
    }

    return fonts.at(key); // SHOULD NEVER REACH HERE!
}

sf::Texture &ResourcePack::getTexture(const std::string &key)
{
    try
    {
        return textures.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent texture ") + key + _(" in resource pack") + name);
    }

    return textures.at(key); // SHOULD NEVER REACH HERE!
}