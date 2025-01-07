#pragma once

namespace GUI
{
static float percent(float value, float percent)
{
    return value * (percent / 100.f);
}

static unsigned int charSize(sf::VideoMode &vm, unsigned int divisor = 60)
{
    return static_cast<unsigned int>((vm.size.x + vm.size.y) / divisor);
}
} // namespace GUI