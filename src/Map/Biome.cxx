#include "Map/Biome.hxx"
#include "stdafx.hxx"

Biome::Biome(std::string name, const float ideal_height, const float ideal_moisture, const float ideal_heat)
{
    this->name = name;
    idealHeight = ideal_height;
    idealMoisture = ideal_moisture;
    idealHeat = ideal_heat;
}

Biome::~Biome()
{
}

const bool Biome::favorableConditions(const float height, const float moisture, const float heat)
{
    return height >= idealHeight && moisture >= idealMoisture && heat >= idealMoisture;
}

float Biome::calculateWeight(float height, float moisture, float heat) const
{
    float height_diff = std::abs(height - idealHeight);
    float moisture_diff = std::abs(moisture - idealMoisture);
    float heat_diff = std::abs(heat - idealHeat);

    // Invert differences to create a weight
    return 1.0f / (1.0f + height_diff + moisture_diff + heat_diff);
}

const std::string &Biome::getName() const
{
    return name;
}