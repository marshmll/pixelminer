#include "Tools/UUID.hxx"
#include "stdafx.hxx"

UUID::UUID() = default;

UUID::~UUID() = default;

const std::string UUID::generate()
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    std::string uuid;
    for (int i = 0; i < 16; i++)
    {
        if (dash[i])
            uuid += "-";
        uuid += hex_digits[dist(rng)];
        uuid += hex_digits[dist(rng)];
    }

    return uuid;
}