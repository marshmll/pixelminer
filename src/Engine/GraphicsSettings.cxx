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
        JSONObject obj = JSON::parse(path).get<JSONObject>();

        screenWidth = obj.at("resolution").get<JSONObject>().at("width").get<long long int>();
        screenHeight = obj.at("resolution").get<JSONObject>().at("height").get<long long int>();
        framerateLimit = obj.at("framerateLimit").get<long long int>();
        fullscreen = obj.at("fullscreen").get<bool>();
        vsync = obj.at("vsync").get<bool>();

        std::cout << "[ GraphicsSettings ] -> Loaded settings from file: " << path.string() << "\n";
        return true;
    }
    catch (std::exception e)
    {
        std::cerr << "[ GraphicsSettings ] -> Could not load settings from file " << path << " : \"" << e.what() << "\""
                  << "\n";
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

    JSONObject obj;
    obj["resolution"] = JSONObject{{"width", screenWidth}, {"height", screenHeight}};
    obj["framerateLimit"] = framerateLimit;
    obj["fullscreen"] = fullscreen;
    obj["vsync"] = vsync;

    try
    {
        out << JSON::stringify(obj);
    }
    catch (std::exception e)
    {
        out.close();
        std::cerr << "[ GraphicsSettings ] -> Error while saving to file: \"" << e.what() << "\"\n";
        return false;
    }

    out.close();
    std::cout << "[ GraphicsSettings ] -> Saved settings to file: " << path.string() << "\n";
    return true;
}