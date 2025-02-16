#include "Map/Biome.hxx"
#include "stdafx.hxx"

void Biome::setBiomeName()
{
    switch (type)
    {
    case Desert: name = _("Desert"); break;
    case Forest: name = _("Forest"); break;
    case Grassland: name = _("Grassland"); break;
    case Jungle: name = _("Jungle"); break;
    case Mountains: name = _("Mountains"); break;
    case Ocean: name = _("Ocean"); break;
    case Tundra: name = _("Tundra"); break;
    default: name = _("Unknown"); break;
    }
}

Biome::Biome(const BiomeType type, const float &ideal_height, const float &ideal_moisture, const float &ideal_heat)
    : type(type), idealHeight(ideal_height), idealMoisture(ideal_moisture), idealHeat(ideal_heat)
{
    setBiomeName();
}

Biome::~Biome()
{}

const float Biome::calculateWeight(const float &height, const float &moisture, const float &heat) const
{
    float height_diff = std::abs(height - idealHeight);
    float moisture_diff = std::abs(moisture - idealMoisture);
    float heat_diff = std::abs(heat - idealHeat);

    // Invert differences to create a weight
    return 1.0f / (1.0f + height_diff + moisture_diff + heat_diff);
}

const BiomeType &Biome::getType() const
{
    return type;
}

const std::string &Biome::getName() const
{
    return name;
}