#include "Entities/Functionalities/AttributeFunctionality.hxx"
#include "stdafx.hxx"

AttributeFunctionality::AttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger)
    : health(max_health), maxHealth(max_health), hunger(max_hunger), maxHunger(max_hunger)
{
}

AttributeFunctionality::~AttributeFunctionality()
{
}

/* ACCESSORS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

const uint8_t AttributeFunctionality::getHealth() const
{
    return health;
}

const uint8_t AttributeFunctionality::getHunger() const
{
    return hunger;
}

const uint8_t AttributeFunctionality::getMaxHealth() const
{
    return maxHealth;
}

const uint8_t AttributeFunctionality::getMaxHunger() const
{
    return maxHunger;
}

/* MODIFIERS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AttributeFunctionality::setHealth(const uint8_t health)
{
    this->health = health;
}

void AttributeFunctionality::setHunger(const uint8_t hunger)
{
    this->hunger = hunger;
}

void AttributeFunctionality::setMaxHealth(const uint8_t max_health)
{
    this->maxHealth = max_health;
}

void AttributeFunctionality::setMaxHunger(const uint8_t max_hunger)
{
    this->maxHunger = max_hunger;
}
