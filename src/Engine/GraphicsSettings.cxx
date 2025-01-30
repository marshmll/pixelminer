#include "Engine/GraphicsSettings.hxx"
#include "stdafx.hxx"

GraphicsSettings::GraphicsSettings()
{
}

GraphicsSettings::~GraphicsSettings()
{
}

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

        std::cout << "[ GraphicsSettings ] -> Loaded settings from file: " << path.string() << "\n";
        return true;
    }
    catch (std::runtime_error e)
    {
        std::cerr << "[ GraphicsSettings ] -> Could not load settings from file " << path << "\n" << e.what();
    }

    return false;
}

const bool GraphicsSettings::saveToFile(const std::filesystem::path path)
{
    std::ofstream out(path);

    if (!out.is_open())
    {
        std::cout << "[ GraphicsSettings ] -> Could not save settings to file " << path.string() << "\n";
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
        std::cerr << "[ GraphicsSettings ] -> Error while saving to file: \"" << e.what() << "\"\n";
        return false;
    }

    out.close();
    std::cout << "[ GraphicsSettings ] -> Saved settings to file: " << path.string() << "\n";
    return true;
}