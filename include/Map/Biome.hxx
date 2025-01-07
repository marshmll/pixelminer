#pragma once

class Biome
{
  private:
    std::string name;
    float idealHeight;
    float idealMoisture;
    float idealHeat;

  public:
    Biome(std::string name, const float ideal_height, const float ideal_moisture, const float ideal_heat);
    ~Biome();

    const bool favorableConditions(const float height, const float moisture, const float heat);

    float calculateWeight(float height, float moisture, float heat) const;

    const std::string &getName() const;
};