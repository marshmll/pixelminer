#include "Engine/ResourcePack.hxx"
#include "stdafx.hxx"

ResourcePack::ResourcePack() : logger("ResourcePack")
{}

ResourcePack::~ResourcePack()
{
    deleteCache();
}

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

    std::ifstream pack_file(root_path / "pack.json");

    if (!pack_file.is_open())
    {
        logger.logError(_("Failed to open \"pack.json\" file in resource pack: ") + root_path.string(), false);
        return false;
    }

    std::stringstream pack_stream;
    pack_stream << pack_file.rdbuf();

    JObject pack_obj = JSON::parse(pack_stream.str()).getAs<JObject>();

    pack_file.close();

    /* Name and description +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    this->name = pack_obj.at("name").getAs<std::string>();
    this->description = pack_obj.at("description").getAs<std::string>();

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
    }

    /* Default images +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    std::ifstream images_file(root_path / "Assets/Textures/textures.json");

    if (!images_file.is_open())
    {
        logger.logError(_("Failed to open \"textures.json\" file in resource pack: ") + root_path.string(), false);
        return false;
    }

    std::stringstream images_ss;
    images_ss << images_file.rdbuf();
    images_file.close();

    JArray images_array = JSON::parse(images_ss.str()).getAs<JArray>();

    // Load textures from images.json
    for (auto &val : images_array)
    {
        JObject entry = val.getAs<JObject>();
        std::string key = entry.at("key").getAs<std::string>(), path = entry.at("path").getAs<std::string>();

        if (!textures[key].loadFromFile(root_path / path))
            logger.logWarning(_("Missing texture ") + path + _(" in resource pack: ") + name);
    }

    /* Default Sounds +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    std::ifstream sounds_file(root_path / "Assets/Sounds/sounds.json");

    if (!sounds_file.is_open())
    {
        logger.logError(_("Failed to open \"sounds.json\" file in resource pack: ") + root_path.string(), false);
        return false;
    }

    std::stringstream sounds_ss;
    sounds_ss << sounds_file.rdbuf();
    sounds_file.close();

    JObject sounds_obj = JSON::parse(sounds_ss.str()).getAs<JObject>();

    // Sounds
    for (auto &val : sounds_obj.at("effects").getAs<JArray>())
    {
        JObject obj = val.getAs<JObject>();
        std::string key = obj.at("key").getAs<std::string>(), path = obj.at("path").getAs<std::string>();
        const bool global = obj.at("global").getAs<bool>();

        if (!soundBuffers[key].loadFromFile(root_path / path))
        {
            logger.logWarning(_("Missing sound ") + path + _(" in resource pack: ") + name);
            continue;
        }

        if (global)
            globalSounds[key] = std::make_shared<sf::Sound>(soundBuffers.at(key));
    }

    // Music
    for (auto &val : sounds_obj.at("musics").getAs<JArray>())
    {
        JObject obj = val.getAs<JObject>();
        std::string key = obj.at("key").getAs<std::string>(), path = obj.at("path").getAs<std::string>();

        if (!std::filesystem::exists(root_path / path))
            logger.logWarning(_("Missing music ") + path + _(" in resource pack: ") + name);
        else
            musics[key] = std::make_shared<sf::Music>(root_path / path);
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

            std::string tag, name;
            int rect_x, rect_y, size;

            tag = obj.at("tag").getAs<std::string>();
            name = obj.at("name").getAs<std::string>();
            rect_x = static_cast<int>(obj.at("rectX").getAs<long long>());
            rect_y = static_cast<int>(obj.at("rectY").getAs<long long>());
            size = static_cast<int>(obj.at("size").getAs<long long>());

            tileDb.insert(tag, name, rect_x, rect_y, size);
        }
        catch (std::runtime_error &e)
        {
            logger.logError(_("Error while reading tile database: ") + static_cast<std::string>(e.what()), false);
            return false;
        }
    }

    logger.logInfo(_("Sucessfully loaded resource pack ") + name + ": " + std::to_string(textures.size()) +
                   _(" textures, ") + std::to_string(fonts.size()) + _(" fonts, ") +
                   std::to_string(soundBuffers.size()) + _(" sound buffers, ") + std::to_string(globalSounds.size()) +
                   _(" sounds, ") + std::to_string(musics.size()) + _(" musics."));

    return true;
}

void ResourcePack::deleteCache()
{
    if (name.size() != 0 && std::filesystem::exists(CACHE_FOLDER + "ResourcePacks/" + name))
        std::filesystem::remove_all(CACHE_FOLDER + "ResourcePacks/" + name);
}

sf::Font &ResourcePack::getFont(const std::string &key)
{
    try
    {
        return fonts.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent font ") + key + _(" in resource pack: ") + name);
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
        logger.logError(_("Inexistent texture ") + key + _(" in resource pack: ") + name);
    }

    return textures.at(key); // SHOULD NEVER REACH HERE!
}

sf::SoundBuffer &ResourcePack::getSoundBuffer(const std::string &key)
{
    try
    {
        return soundBuffers.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent sound buffer") + key + _(" in resource pack: ") + name);
    }

    return soundBuffers.at(key); // SHOULD NEVER REACH HERE!
}

sf::Sound &ResourcePack::getSound(const std::string &key)
{
    try
    {
        return *globalSounds.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent sound ") + key + _(" in resource pack: ") + name);
    }

    return *globalSounds.at(key); // SHOULD NEVER REACH HERE!
}

sf::Music &ResourcePack::getMusic(const std::string &key)
{
    try
    {
        return *musics.at(key);
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent music ") + key + _(" in resource pack: ") + name);
    }

    return *musics.at(key); // SHOULD NEVER REACH HERE!
}

void ResourcePack::stopAllMusics()
{
    for (auto &[_, music] : musics)
        music->stop();
}

void ResourcePack::setFontSmoothness(const bool smoothness)
{
    for (auto &[_, font] : fonts)
        font.setSmooth(smoothness);
}

void ResourcePack::setTextureSmoothness(const bool smoothness)
{
    for (auto &[_, texture] : textures)
        texture.setSmooth(smoothness);
}
