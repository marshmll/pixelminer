#include "Engine/GraphicsSettings.hxx"
#include "stdafx.hxx"

GraphicsSettings::GraphicsSettings() : logger("GraphicsSettings")
{}

GraphicsSettings::~GraphicsSettings() = default;

const bool GraphicsSettings::loadFromFile(const std::filesystem::path path)
{
    try
    {
        JObject obj = JSON::parse(path).getAs<JObject>();
        JObject resolutionObj = obj.at("resolution").getAs<JObject>();

        screenWidth = resolutionObj.size() == 2 ? resolutionObj.at("width").getAs<long long int>()
                                                : sf::VideoMode::getDesktopMode().size.x;

        screenHeight = resolutionObj.size() == 2 ? resolutionObj.at("height").getAs<long long int>()
                                                 : sf::VideoMode::getDesktopMode().size.y;

        framerateLimit = obj.at("framerateLimit").getAs<long long int>();

        fullscreen = obj.at("fullscreen").getAs<bool>();

        resourcePack = obj.at("resourcePack").getAs<std::string>();

        logger.logInfo("Loaded settings from file: " + path.string());
        return true;
    }
    catch (std::runtime_error e)
    {
        logger.logWarning("Failed to load settings from file " + path.string() + "\n" + e.what());
    }

    return false;
}

const bool GraphicsSettings::saveToFile(const std::filesystem::path path)
{
    std::ofstream out(path);

    if (!out.is_open())
    {
        logger.logWarning("Failed to save settings to file " + path.string());
        return false;
    }

    JObject obj;
    obj["resolution"] = JObject{{"width", screenWidth}, {"height", screenHeight}};
    obj["framerateLimit"] = framerateLimit;
    obj["fullscreen"] = fullscreen;
    obj["resourcePack"] = resourcePack;

    try
    {
        out << JSON::stringify(obj);
    }
    catch (std::runtime_error e)
    {
        out.close();
        logger.logWarning("Error while saving to file: \"" + static_cast<std::string>(e.what()) + "\"");
        return false;
    }

    out.close();
    logger.logInfo("Saved settings to file: " + path.string());
    return true;
}